#pragma once

#include <string>
#include "PixelEngine.h"
#include "UI/UI.h"

class ContentBrowser : public UI
{
public:
	ContentBrowser() : UI("Content Browser")
    {
        ConstructIcons("assets/icons");
    }

	~ContentBrowser() {}

    void SetCurrentDirectory(const std::string& directory) {
        _currentDirectory = directory;
    }

	void OnGUI() override;

private:
    std::string _currentDirectory;
    std::map<std::string, Texture*> _iconTextures;

    void ConstructDirectoryPath(const std::string& directory, const std::string& projectName);

    void ConstructIcons(std::string directory);
    void ConstructDirectoryTree(const std::string& directory, int level, bool isRoot);
    void ConstructContextMenu(const std::string& directory);
    void ConstructAssetGrid(const std::string& directory);

    std::string ResolveIcons(const std::string& extension);
};