#pragma once

#include "pch.h"
#include "PixelEngine.h"

class Project : public Asset
{
public:
	Project() : Asset(AssetInfo(), AssetType::Project) {}
	Project(std::string path) : Asset(AssetInfo(path), AssetType::Project) {}
	Project(std::string projectName, std::string directory) : Asset(AssetInfo(directory + "\\" + projectName + "\\.pixellab" ), AssetType::Project) {
        _info.SetName(projectName);
    }
	~Project() = default;
};