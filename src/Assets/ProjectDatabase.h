#pragma once

#include "pch.h"
#include "Project.h"
#include "ProjectList.h"
#include <Windows.h>
#include <shlobj.h>
#include <filesystem>

class ProjectDatabase
{
public:
	static ProjectDatabase* GetInstance() {
		return &_instance;
	}

	void InitializeDatabase();

	void AddProject(Project* project);
	void RemoveProject(Project* project);

	Project* GetProject(std::string projectName);
	Project* GetProject(int index);
	std::vector<Project*> GetProjects() { return _projectList.GetProjects(); }

	int GetProjectCount() { return _projectList.GetProjectCount(); }
	void SerializeDatabase();
private:
	static ProjectDatabase _instance;
	ProjectList _projectList;
	std::string _databasePath = "C:\\ProgramData\\PixelLab";;
};