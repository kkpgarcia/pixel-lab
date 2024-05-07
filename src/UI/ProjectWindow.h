#pragma once

#include "PixelEngine.h"
#include "Assets/Project.h"
#include "UI.h"
#include "Windows.h"
#include "shlobj.h"
#include <filesystem>
#include "Editor.h"

#include "Assets/ProjectDatabase.h"

class ProjectWindow : public UI
{
public:
	ProjectWindow();// : UI("Project Window") {}
	~ProjectWindow() {}

	void OnGUI() override;

private:
	bool _isVisible = true;
	bool _showCreateProjectDialog = false;
	bool _showOpenProjectDialog = false;
	//void ShowOpenProjectDialog(bool* show);
	void ShowCreateProjectDialog(bool* show);
	void OpenProject(std::string path);
	void CreateProject(std::string projectName, std::string path);
	std::string BrowseFolder(const std::string& initialPath);
};