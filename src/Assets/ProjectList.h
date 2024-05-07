#pragma once

#include "Project.h"
#include <filesystem>

class ProjectList : public ISerializable
{
public:
	void AddProject(Project* project) { _projects.push_back(project); }
	void RemoveProject(Project* project) 
	{
		_projects.erase(std::remove(_projects.begin(), _projects.end(), project), _projects.end());
	}

	Project* GetProject(std::string projectName)
	{
		for (Project* project : _projects)
		{
			if (project->GetName() == projectName)
			{
				return project;
			}
		}

		return nullptr;
	}

	Project* GetProject(int index)
	{
		if (index < _projects.size())
		{
			return _projects[index];
		}

		return nullptr;
	}

	std::vector<Project*> GetProjects() { return _projects; }

	int GetProjectCount() { return _projects.size(); }

	void Serialize(YAML::Emitter& out) const override
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Projects"; out << YAML::Value << YAML::BeginSeq;
		for (Project* project : _projects)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Project Name"; out << YAML::Value << project->GetName();
			out << YAML::Key << "Project Path"; out << YAML::Value << project->GetPath();
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}

	void Deserialize(YAML::Node& root) override
	{
		YAML::Node projects = root["Projects"];
		for (YAML::Node project : projects)
		{
			std::string projectName = project["Project Name"].as<std::string>();
			std::string projectPath = project["Project Path"].as<std::string>();

			if (std::filesystem::exists(projectPath + projectName + "/project.pixellab"))
			{
				Project* project = new Project();

				YAML::Node projectYaml = YAML::LoadFile(projectPath + projectName + "/project.pixellab");
				project->Deserialize(projectYaml);

				//YAMLDeserializer projectDeserializer;
				//projectDeserializer.Deserialize(*project, projectPath + projectName + "/project.pixellab");

				if (project->IsEmpty()) {
					delete project;
					continue;
				}

				_projects.push_back(project);
			}
		}
	}
private:
	std::vector<Project*> _projects;
};