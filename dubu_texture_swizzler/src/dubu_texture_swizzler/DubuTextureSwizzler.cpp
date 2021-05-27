#include "DubuTextureSwizzler.hpp"

#include <dubu_texture_swizzler/stb_image.h>
#include <dubu_texture_swizzler/stb_image_write.h>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

static constexpr const char* POPUP_PROGRESS = "Sizzling and Swizzling...";

static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(),
	                     s.rend(),
	                     [](unsigned char ch) { return !std::isspace(ch); })
	            .base(),
	        s.end());
}
static inline void trim(std::string& s) {
	rtrim(s);
}

DubuTextureSwizzler::DubuTextureSwizzler()
    : dubu::opengl_app::AppBase({.width        = 500,
                                 .height       = 150,
                                 .appName      = "Dubu Texture Swizzler",
                                 .swapInterval = 1}) {}

void DubuTextureSwizzler::Init() {}

void DubuTextureSwizzler::Update() {
	if (mSwizzling) {
		if (mSwizzling->index >= mTextureFiles.size()) {
			mSwizzling = std::nullopt;
		} else {
			SwizzleTexture(mTextureFiles[mSwizzling->index]);
			++mSwizzling->index;
		}
	}

	DrawDockSpace();

	if (ImGui::Begin("Texture Swizzler",
	                 nullptr,
	                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
	                     ImGuiWindowFlags_NoMove)) {
		if (mSwizzling) {
			if (!ImGui::IsPopupOpen(POPUP_PROGRESS)) {
				ImGui::OpenPopup(POPUP_PROGRESS);
			}
			if (ImGui::BeginPopupModal(
			        POPUP_PROGRESS, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				int        current     = mSwizzling->index;
				int        numTextures = static_cast<int>(mTextureFiles.size());
				const auto text =
				    fmt::format("Texture {}/{}", current, numTextures);
				float progress = current / static_cast<float>(numTextures);

				ImGui::SetNextItemWidth(200.f);
				ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), text.c_str());

				if (ImGui::Button("Cancel", ImVec2(200.f, 0))) {
					mSwizzling = std::nullopt;
				}
				ImGui::EndPopup();
			}
		}

		if (ImGui::InputText("Directory Path", &mDirectoryString)) {
			OnDirectoryPathUpdated();
		}

		if (ImGui::BeginCombo("Channels to Swap",
		                      mSwapCombinations[mCurrentSwap])) {
			for (int i = 0; i < mSwapCombinations.size(); ++i) {
				const bool is_selected = (mCurrentSwap == i);
				if (ImGui::Selectable(mSwapCombinations[i], is_selected)) {
					mCurrentSwap = i;
				}
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Found %d image files.", mTextureFiles.size());

		if (ImGui::Button("Swizzle")) {
			OnSwizzle();
		}
	}
	ImGui::End();
}

void DubuTextureSwizzler::OnDirectoryPathUpdated() {
	mTextureFiles.clear();

	trim(mDirectoryString);

	auto path = std::filesystem::path(mDirectoryString);

	if (!std::filesystem::is_directory(path)) {
		return;
	}

	static const std::vector<std::filesystem::path> ValidExtensions{
	    ".tga",
	    ".TGA",
	    ".png",
	    ".PNG",
	    ".bmp",
	    ".BMP",
	};

	for (auto& file : std::filesystem::directory_iterator(path)) {
		auto& p = file.path();
		if (p.has_extension() &&
		    std::find(ValidExtensions.begin(),
		              ValidExtensions.end(),
		              p.extension()) != ValidExtensions.end()) {
			mTextureFiles.push_back(p);
		}
	}
}

void DubuTextureSwizzler::OnSwizzle() {
	mSwizzling = Swizzling{
	    .index    = 0,
	    .channel1 = ChannelCharToIndex(mSwapCombinations[mCurrentSwap][0]),
	    .channel2 = ChannelCharToIndex(mSwapCombinations[mCurrentSwap][1]),
	};
}

void DubuTextureSwizzler::SwizzleTexture(std::filesystem::path& p) {
	auto filepath = p.generic_string();
	int  w, h, c;
	auto pixels = stbi_load(filepath.c_str(), &w, &h, &c, 0);
	if (!pixels) {
		return;
	}

	int channel1 = mSwizzling->channel1;
	int channel2 = mSwizzling->channel2;

	if (c >= std::max(channel1, channel2) + 1) {
		for (int i = 0; i < w * h; ++i) {
			auto pixel = pixels + i * c;
			std::swap(*(pixel + channel1), *(pixel + channel2));
		}
	}

	if (p.extension() == ".tga" || p.extension() == ".TGA") {
		stbi_write_tga(filepath.c_str(), w, h, c, pixels);
	} else if (p.extension() == ".bmp" || p.extension() == ".BMP") {
		stbi_write_bmp(filepath.c_str(), w, h, c, pixels);
	} else if (p.extension() == ".png" || p.extension() == ".PNG") {
		stbi_write_png(filepath.c_str(), w, h, c, pixels, w * c);
	}

	stbi_image_free(pixels);
}

int DubuTextureSwizzler::ChannelCharToIndex(char c) {
	switch (c) {
	case 'R':
		return 0;
	case 'G':
		return 1;
	case 'B':
		return 2;
	default:
		return 3;
	}
}

void DubuTextureSwizzler::DrawDockSpace() {
	ImGuiWindowFlags dockSpaceWindowFlags =
	    ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
	    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
	    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
	    ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
	ImGui::Begin("DOCK_SPACE", nullptr, dockSpaceWindowFlags);

	ImGui::DockSpace(ImGui::GetID("DOCK_SPACE_WINDOW"),
	                 {0.f, 0.f},
	                 ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();
	ImGui::PopStyleVar(3);
}
