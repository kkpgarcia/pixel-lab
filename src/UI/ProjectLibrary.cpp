#include <filesystem>
#include "ProjectLibrary.h"
#include "Editor.h"
#include "File Management/File.h"

void ProjectLibrary::OnGUI()
{
    Project* currentProject = Editor::GetInstance()->GetProject();

    if (currentProject == nullptr)
    {
        ImGui::Text("No project open.");
        return;
    }

    ImGui::Begin("Project Library");

    // Create two columns
    ImGui::Columns(2, "ProjectLibraryColumns", false);

    // Set the width of the first column to be smaller
    ImGui::SetColumnWidth(0, 150.0f);

    ImGui::Text("Directory");
    // Content for the first (smaller) column
    ImGui::BeginChild("Scrolling");\
    ConstructDirectoryTree(currentProject->GetPath(), 0, true);
    ImGui::EndChild();

    ImGui::NextColumn();

    // Content for the second column
    ImGui::Text("Files");

    ConstructAssetGrid(_currentDirectory);
    ImGui::Columns(1); // Reset to one column

    ImGui::End();
}

void ProjectLibrary::ConstructDirectoryTree(const std::string& directory, int level = 0, bool isRoot = true)
{
    // Get all files in the current directory
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        // Check if the file is a directory
        if (entry.is_directory())
        {
            //Check if path has no more directories
            if (std::filesystem::directory_iterator(entry.path()) == std::filesystem::directory_iterator())
            {
                if (!isRoot) {
                    ImGui::Indent(level * 5.0f);
                }
                if (ImGui::Selectable(entry.path().filename().string().c_str()))
                {
                    // Set the current directory to the selected directory
                    _currentDirectory = entry.path().string();
                }
                if (!isRoot) {
                    ImGui::Unindent(level * 5.0f);
                }
            }
            else {
                if (!isRoot) {
                    ImGui::Indent(level * 5.0f);
                }
                if (ImGui::TreeNodeEx(entry.path().filename().string().c_str(), ImGuiTreeNodeFlags_SpanAvailWidth)) {
                    if (ImGui::IsItemClicked())
                    {
                        // Set the current directory to the selected directory
                        _currentDirectory = entry.path().string();
                    }
                    ConstructDirectoryTree(entry.path().string(), level + 1, false);
                    ImGui::TreePop();
                }
                if (!isRoot) {
                    ImGui::Unindent(level * 5.0f);
                }
            }
        }
    }
}

void ProjectLibrary::ConstructAssetGrid(const std::string& directory)
{
    if (directory.empty())
    {
        ImGui::Text("No directory selected.");
        return;
    }

    if (!File::DirectoryExists(directory.c_str()))
    {
        ImGui::Text("Directory does not exist.");
        return;
    }

    // Get all files in the current directory

    auto files = File::GetFilesInDirectory(directory.c_str());

    if (files.empty())
    {
        ImGui::Text("No files in directory.");
        return;
    }

    float windowWidth = ImGui::GetWindowWidth() - 150.0f;
    int columns = windowWidth / 100;
    float spacerSize = (windowWidth - columns * 100) / (columns + 1);

    ImGui::BeginTable("FilesTable", columns);
    int column = 0;

    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            ImGui::Dummy(ImVec2(spacerSize, 0)); // Add spacer before each item
            ImGui::SameLine();
            ImGui::TableNextColumn();
            if (ImGui::Button(entry.path().filename().string().c_str(), ImVec2(100.0f, 100.0f))) {
                // Handle file selection
            }
            if (++column >= columns) {
                column = 0;
                ImGui::TableNextRow();
            }
        }
        else if (entry.is_directory())
        {
            ImGui::Dummy(ImVec2(spacerSize, 0)); // Add spacer before each item
            ImGui::SameLine();
            ImGui::TableNextColumn();
            if (ImGui::Button(entry.path().filename().string().c_str(), ImVec2(100.0f, 100.0f))) {
                _currentDirectory = entry.path().string();
            }
            if (++column >= columns) {
                column = 0;
                ImGui::TableNextRow();
            }
        }
    }

    ImGui::EndTable();
}