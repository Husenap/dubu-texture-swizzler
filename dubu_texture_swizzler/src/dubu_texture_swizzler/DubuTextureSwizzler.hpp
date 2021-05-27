#pragma once

#include <dubu_opengl_app/dubu_opengl_app.hpp>

class DubuTextureSwizzler : public dubu::opengl_app::AppBase {
public:
	DubuTextureSwizzler();
	virtual ~DubuTextureSwizzler() = default;

protected:
	virtual void Init() override;
	virtual void Update() override;

private:
	void OnDirectoryPathUpdated();
	void OnSwizzle();

	void SwizzleTexture(std::filesystem::path &p);

	int ChannelCharToIndex(char c);

	static void DrawDockSpace();

	std::string                        mDirectoryString;
	std::vector<std::filesystem::path> mTextureFiles;
	int                                mCurrentSwap = 0;

	const std::vector<const char *> mSwapCombinations{
	    "RG", "RB", "RA", "GB", "GA", "BA"};

	struct Swizzling {
		int index;
		int channel1;
		int channel2;
	};
	std::optional<Swizzling> mSwizzling;
};