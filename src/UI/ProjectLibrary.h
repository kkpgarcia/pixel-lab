#pragma once

#include <string>
#include "PixelEngine.h"
#include "UI.h"

class ProjectLibrary : public UI
{
public:
	ProjectLibrary() : UI("Project Library") {}
	~ProjectLibrary() {}

    void SetCurrentDirectory(const std::string& directory) {
        _currentDirectory = directory;
    }

	void OnGUI() override;

private:
    std::string _currentDirectory;

    void ConstructDirectoryTree(const std::string& directory, int level, bool isRoot);
    void ConstructAssetGrid(const std::string& directory);
};