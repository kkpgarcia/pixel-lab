#pragma once

#include "pch.h"
#include "PixelEngine.h"

class Project : public Asset
{
public:
	Project() : Asset(AssetInfo(), AssetType::Project) {}
	Project(std::string path) : Asset(AssetInfo(path), AssetType::Project) {}
	Project(std::string projectName, std::string directory) : Asset(AssetInfo(), AssetType::Project)
    {
        _info.SetName(projectName);
        _info.SetPath(directory + "\\" + projectName);
        _info.SetExtension("pixellab");
        _type = AssetType::Project;
    }
	~Project() = default;

	std::string GetFullPath() const override {
		return GetPath() + "\\project." + _info.GetExtension();
	}

	void Serialize(YAML::Emitter& out) const override
	{
		Asset::Serialize(out);
	}

	void Deserialize(YAML::Node& root) override
	{
		Asset::Deserialize(root);
	}

	void Save();
};