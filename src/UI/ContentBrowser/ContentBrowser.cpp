#include "ContentBrowser.h"
#include <filesystem>
#include "Editor.h"

void ContentBrowser::ConstructIcons(std::string directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        std::string path = entry.path().string();
        std::string filename = entry.path().filename().string();
        if (filename.find(".png") != std::string::npos)
        {
            auto* texture = AssetManager::GetInstance()->Load<Texture>(path);

            _iconTextures[filename] = texture;
        }
    }
}

void ContentBrowser::OnGUI()
{
    Project* currentProject = Editor::GetProject();

    if (currentProject == nullptr)
    {
        ImGui::Text("No project open.");
        return;
    }

    ImGui::Columns(2, "ProjectLibraryColumns", false);
    ImGui::SetColumnWidth(0, 150.0f);

    ImGui::Text("Directory");
    ImGui::BeginChild("Scrolling");
    ConstructDirectoryTree(currentProject->GetPath(), 0, true);
    ImGui::EndChild();

    ImGui::NextColumn();

    ImGui::Text("Files");
    ImGui::SameLine();

    ConstructDirectoryPath(_currentDirectory, currentProject->GetName());


    ConstructAssetGrid(_currentDirectory);
    ImGui::Columns(1);
}

void ContentBrowser::ConstructDirectoryPath(const std::string& directory, const std::string& projectName)
{
    std::vector<std::string> stringTokens = StringUtility::SplitString(directory, "\\");

    int index = 0;
    for (int i = 0; i < stringTokens.size(); i++)
    {
        if (stringTokens[i] == projectName)
        {
            index = i;
            break;
        }
    }

    std::string baseDirectory;

    for (int i = 0; i < index; i++)
    {
        baseDirectory += stringTokens[i] + "\\";
    }

    stringTokens.erase(stringTokens.begin(), stringTokens.begin() + index);

    for (int i = 0; i < stringTokens.size(); i++)
    {
        auto token = stringTokens[i];

        if (token == projectName) continue;

        if (i > 0)
        {
            ImGui::SameLine();
            ImGui::Text(">");
            ImGui::SameLine();
        }

        if (ImGui::Button(stringTokens[i].c_str()))
        {
            std::string path;
            for (int j = 0; j <= i; j++)
            {
                if (j > 0)
                    path += "\\";
                path += stringTokens[j];
            }

            _currentDirectory = baseDirectory + path;
        }
    }
}

void ContentBrowser::ConstructDirectoryTree(const std::string& directory, int level = 0, bool isRoot = true)
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

                if (_currentDirectory == entry.path().string()) {
                    ConstructContextMenu(entry.path().string());
                }

                if (nodeOpen) {
                    ConstructDirectoryTree(entry.path().string(), level + 1, false);
                    ImGui::TreePop();
                }
            } else {
                if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_None)) {
                    _currentDirectory = entry.path().string();
                }

                if (_currentDirectory == entry.path().string()) {
                    ConstructContextMenu(entry.path().string());
                }
            }

            if (!isRoot) {
                ImGui::Unindent(level * 5.0f);
            }
        }
    }
}

void ContentBrowser::ConstructContextMenu(const std::string &directory) {
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::MenuItem("Create Folder"))
        {
            std::string newDirectory = directory + "/New Folder";
            std::filesystem::create_directory(newDirectory);
        }

        if (ImGui::MenuItem("Open in Explorer"))
        {
            std::string command = "explorer " + directory;
            system(command.c_str());
        }

        if (ImGui::MenuItem("Rename"))
        {

        }

        if (ImGui::MenuItem("Delete"))
        {
            if (directory.find("/Assets") != std::string::npos && directory != "/Assets")
            {
                if (ImGui::BeginPopup("Delete Directory"))
                {
                    ImGui::Text("Are you sure you want to delete this directory?");
                    if (ImGui::Button("Yes"))
                    {
                        std::filesystem::remove_all(directory);
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("No"))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
        }
        ImGui::EndPopup();
    }
}

void ContentBrowser::ConstructAssetGrid(const std::string& directory)
{
    if (directory.empty())
    {
        ImGui::Text("No directory selected.");
        return;
    }

    if (!std::filesystem::exists(directory))
    {
        ImGui::Text("Directory does not exist.");
        return;
    }

    // Get all files in the current directory

    auto files = std::filesystem::directory_iterator(directory);

    if (files == std::filesystem::directory_iterator())
    {
        ImGui::Text("No files in directory.");
        return;
    }

    float windowWidth = ImGui::GetWindowWidth() - 150.0f;
    int columns = windowWidth / 100;
    float spacerSize = (windowWidth - columns * 100) / (columns + 1);

    ImGui::BeginTable("FilesTable", columns);
    int column = 0;

    int id = 0;
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        ImGui::PushID(id++);
        ImGui::Dummy(ImVec2(spacerSize, 0)); // Add spacer before each item
        ImGui::TableNextColumn();

        std::string extension = entry.path().extension().string();

        ImGui::BeginGroup();
        Texture* iconTexture;
        if (entry.is_regular_file())
        {
            extension = ResolveIcons(extension);
            iconTexture = _iconTextures[extension + ".png"];

            if (iconTexture == 0)
            {
                iconTexture = _iconTextures["unknown.png"];
            }

            ImGui::ImageButton((void*)(intptr_t)iconTexture->GetID(), ImVec2(100, 100));

        }
        else if (entry.is_directory())
        {
            iconTexture = _iconTextures["folder.png"];
            if (ImGui::ImageButton((void*)(intptr_t)iconTexture->GetID(), ImVec2(100, 100)))
            {
                _currentDirectory = entry.path().string();
            }
        }

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", entry.path().string().c_str(), entry.path().string().size(), ImGuiCond_Once);
            ImGui::ImageButton((void*)(intptr_t)iconTexture->GetID(), ImVec2(50, 50));
            ImGui::Text("%s", entry.path().filename().string().c_str());
            ImGui::EndDragDropSource();
        }

        ImGui::Text("%s", entry.path().filename().string().c_str());
        ImGui::EndGroup();

        if (++column >= columns) {
            column = 0;
            ImGui::TableNextRow();
        }
        ImGui::PopID();
    }

    ImGui::EndTable();
}

std::string ContentBrowser::ResolveIcons(const std::string &extension) {

    if (_iconTextures.count(extension) > 0) {
        return extension;
    }

    if (extension.empty()) {
        return "unknown";
    }

    if (extension == ".cpp" || extension == ".h" || extension == ".hpp") {
        return "cpp";
    }

    if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
        return "image";
    }

    if (extension == ".text" || extension == ".txt" || extension == ".md") {
        return "text";
    }

    if (extension == ".vert" || extension == ".frag" || extension == ".shader") {
        return "shader";
    }

    return extension;
}