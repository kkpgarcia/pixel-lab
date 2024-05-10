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
        if (entry.is_directory()) {
            if (!isRoot) {
                ImGui::Indent(level * 5.0f);
            }

            bool hasSubdirectories = std::any_of(
                    std::filesystem::directory_iterator(entry.path()),
                    std::filesystem::directory_iterator(),
                    [](const auto& e) { return e.is_directory(); }
            );

            if (hasSubdirectories) {
                std::string nodeLabel = "##" + entry.path().string();
                bool nodeOpen = ImGui::TreeNodeEx(nodeLabel.c_str(), ImGuiTreeNodeFlags_None);
                ImGui::SameLine();
                if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_None)) {
                    _currentDirectory = entry.path().string();
                }

                if (nodeOpen) {
                    ConstructDirectoryTree(entry.path().string(), level + 1, false);
                    ImGui::TreePop();
                }
            } else {
                if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_None)) {
                    _currentDirectory = entry.path().string();
                }
            }

            if (!isRoot) {
                ImGui::Unindent(level * 5.0f);
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
        ImGui::Dummy(ImVec2(spacerSize, 0)); // Add spacer before each item
        ImGui::SameLine();
        ImGui::TableNextColumn();

        if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_None, ImVec2(100.0f, 100.0f))) {
            // Handle file selection
            if (entry.is_regular_file())
            {

            }
            else if (entry.is_directory())
            {
                _currentDirectory = entry.path().string();
            }
        }

        if (++column >= columns) {
            column = 0;
            ImGui::TableNextRow();
        }
    }

    ImGui::EndTable();
}