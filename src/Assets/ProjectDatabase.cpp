#include "ProjectDatabase.h"

ProjectDatabase ProjectDatabase::_instance = ProjectDatabase();

void ProjectDatabase::InitializeDatabase()
{
	int size = MultiByteToWideChar(CP_UTF8, 0, _databasePath.c_str(), -1, NULL, 0);
	wchar_t* wpath = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, _databasePath.c_str(), -1, wpath, size);

	if (!CreateDirectory(reinterpret_cast<LPCSTR>(wpath), NULL))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			std::cout << "Failed to create directory: " << _databasePath << std::endl;
		}
	}
	else
	{
		std::cout << "Created directory: " << _databasePath << std::endl;
	}

	if (std::filesystem::exists(_databasePath + "\\projects.yaml"))
	{
		YAML::Node data = YAML::LoadFile(_databasePath + "\\projects.yaml");
		_projectList.Deserialize(data);
	}
}

void ProjectDatabase::AddProject(Project* project)
{
	if (GetProject(project->GetName()) != nullptr)
	{
		return;
	}

	ProjectDatabase::_projectList.AddProject(project);
}

void ProjectDatabase::RemoveProject(Project* project)
{
	ProjectDatabase::_projectList.RemoveProject(project);
}

Project* ProjectDatabase::GetProject(std::string projectName)
{
	return ProjectDatabase::_projectList.GetProject(projectName);
}

Project* ProjectDatabase::GetProject(int index)
{
	return ProjectDatabase::_projectList.GetProject(index);
}

void ProjectDatabase::SerializeDatabase()
{
	YAML::Emitter emitter;
	_projectList.Serialize(emitter);

	std::ofstream fout(_databasePath + "\\projects.yaml");
	fout << emitter.c_str();
}

