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

	int ChannelCharToIndex(char c);

	static void DrawDockSpace();

	std::string                        mDirectoryString;
	std::vector<std::filesystem::path> mTextureFiles;
	int                                mCurrentSwap = 0;

	const std::vector<const char*> mSwapCombinations{
	    "RG", "RB", "RA", "GB", "GA", "BA"};
};