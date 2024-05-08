#include "ProjectWindow.h"

ProjectWindow::ProjectWindow() : UI("Project")
{
}

void ProjectWindow::OnGUI()
{
	if (ImGui::Begin("Project", &_isVisible))
	{
		ImGui::Text("Pixel Lab");
		ImGui::Text("Version 0.1.0");
		ImGui::Separator();

		ImGui::Spacing();

		ImGui::Text("Recent Projects");

        // Start a scrollable child region
        ImGui::BeginChild("Scrolling", ImVec2(0, 150), true);

        for (Project* project : ProjectDatabase::GetInstance()->GetProjects())
        {
            if (ImGui::Selectable(project->GetName().c_str()))
            {
                OpenProject(project->GetPath() + project->GetName());
            }
        }

        // End of scrollable child region
        ImGui::EndChild();

		ImGui::Spacing();


		if (ImGui::Button("Open Project"))
		{
            std::string path = BrowseFolder("C:");

			if (path.empty()) 
            {
				OpenProject(path);
			}
		}
		if (ImGui::Button("Create Project"))
		{
			_showCreateProjectDialog = true;
		}

		if (_showCreateProjectDialog)
		    ShowCreateProjectDialog(&_showCreateProjectDialog);
	}

    ImGui::End();

}

void ProjectWindow::ShowCreateProjectDialog(bool* p_open) {
    if (!p_open || !*p_open)
        return;

    if (ImGui::Begin("Create New Project", p_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove)) {
        static char projectName[128] = ""; // Buffer for project name input

        // Project Name Input
        ImGui::InputText("Project Name", projectName, IM_ARRAYSIZE(projectName));
        ImGui::SameLine();
        // Create and Cancel Buttons
        if (ImGui::Button("Create")) {
			std::string path = BrowseFolder("C:");

            ImGui::CloseCurrentPopup();
            *p_open = false; // Close the window

			if (path.empty() || std::string(projectName).empty()) {
				// Show error message
				MessageBoxW(NULL, L"Please enter a project name and select a location", L"Error", MB_OK | MB_ICONERROR);
			}
            else
            {
                CreateProject(projectName, path);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            *p_open = false; // Close the window
        }
    }
    ImGui::End();
}

void ProjectWindow::OpenProject(std::string path)
{
	if (std::filesystem::exists(path + "/project.pixellab"))
	{
		TextAsset* projectFile = AssetManager::GetInstance()->Load<TextAsset>(path + "/project.pixellab");
		std::string projectYaml = projectFile->GetTextData();

		YAML::Node yamlFile = YAML::Load(projectYaml);
        Project* project = new Project();
        project->Deserialize(yamlFile);

		Editor::GetInstance()->SetProject(project);

		this->_isVisible = false;

		ProjectDatabase::GetInstance()->AddProject(project);
	}
	else
	{
		MessageBoxW(NULL, L"Invalid project", L"Error", MB_OK | MB_ICONERROR);
	}
}

void ProjectWindow::CreateProject(std::string projectName, std::string path)
{
    Project* project = new Project(projectName, path);
    const char *projectPath = (project->GetPath()).c_str();

    std::cout << "Creating project at: " << projectPath << std::endl;

    if (!CreateDirectory(projectPath, NULL))
    {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            MessageBoxW(NULL, L"Project already exists", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
        else
        {
            MessageBoxW(NULL, L"Failed to create project", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
    }
    else
    {
        YAML::Emitter emitter;
        project->Serialize(emitter);

        std::cout << project->GetFullPath() << std::endl;

        std::ofstream fout(project->GetFullPath());
        fout << emitter.c_str();
    }

    //Create project directory
    std::filesystem::create_directory(path + "/" + projectName + "/Assets");

	ProjectDatabase::GetInstance()->AddProject(project);

    Editor::GetInstance()->SetProject(project);

	this->_isVisible = false;
}

std::string ProjectWindow::BrowseFolder(const std::string& initialPath)
{
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = reinterpret_cast<LPCSTR>(L"Browse for folder...");
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lParam = (LPARAM)initialPath.c_str();

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if (pidl != 0)
    {
        // Get the name of the folder and put it in path
        TCHAR path[MAX_PATH];
        if (SHGetPathFromIDList(pidl, path))
        {
            // Free memory used
            IMalloc* imalloc = 0;
            if (SUCCEEDED(SHGetMalloc(&imalloc)))
            {
                imalloc->Free(pidl);
                imalloc->Release();
            }

			std::filesystem::path p = path;
			return p.string();
        }
    }

    return "";  // Return an empty string if no folder was selected
}