#pragma once

#include "pch.h"
#include "PixelEngine.h"

class Project : public Asset
{
public:
	Project() : Asset(AssetMetadata(), AssetType::Project) {}
	Project(std::string path) : Asset(AssetMetadata(path), AssetType::Project) {}
	Project(std::string projectName, std::string directory) : Asset(AssetMetadata(directory + "\\" + projectName + "\\.pixellab" ), AssetType::Project) {
        _info.SetName(projectName);
    }
	~Project() = default;
};