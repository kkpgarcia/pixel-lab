#pragma once

#include "pch.h"
#include "PixelEngine.h"

class Project : public Asset
{
public:
	Project() : Asset(AssetInfo(), AssetType::Project) {}
	Project(std::string path) : Asset(AssetInfo(path), AssetType::Project) {}
	Project(std::string projectName, std::string directory) : Asset(AssetInfo(directory + "\\" + projectName + ".pixellab"), AssetType::Project) {}
	~Project() = default;

	std::string GetPath() {
		return _info.GetPath();
	}

	std::string GetName() {
		return _info.GetName();
	}

	std::string GetFullPath() {
		return _info.GetFullPath();
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