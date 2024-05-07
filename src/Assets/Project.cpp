#include "pch.h"
#include "Project.h"
#include <Windows.h>

void Project::Save()
{
	const CHAR *path = (_info.GetPath() + _info.GetName()).c_str();

	//Validate the path from asset info
	if (strlen(path) == 0)
	{
		MessageBoxW(NULL, L"Project path is invalid", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	//Create a new directory

	if (!CreateDirectory(path, NULL))
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBoxW(NULL, L"Project already exists", L"Error", MB_OK | MB_ICONERROR);
		}
		else
		{
			MessageBoxW(NULL, L"Failed to create project", L"Error", MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		YAML::Emitter emitter;
		this->Serialize(emitter);

		std::cout << path << std::endl;

        const char* suffix = "\\project.pixellab";
        char* newPath = new char[strlen(path) + strlen(suffix) + 1]; // +1 for the null-terminator
        strcpy(newPath, path);
        strcat(newPath, suffix);

		std::ofstream fout(path);
		fout << emitter.c_str();
	}
}