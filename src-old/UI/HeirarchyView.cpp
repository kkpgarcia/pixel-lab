#include "HierarchyView.h"

#include "Editor.h"

void HierarchyView::OnGUI()
{
    auto scene = Editor::GetScene();

    if (scene == nullptr)
    {
        return;
    }

    for (auto& pair : scene->GetEntities())
    {
        auto entity = pair.second;
        std::string name = entity->GetName();

        ImGui::Image((void*)(intptr_t)_iconTextures[ResolveIcons(*entity)]->GetID(), ImVec2(16, 16));
        ImGui::SameLine();

        if (ImGui::Selectable(name.c_str(), Editor::GetCurrentSelection() == entity->GetUUID()))
        {
            Editor::SetCurrentSelection(entity->GetUUID());
        }
    }
}

void HierarchyView::ConstructIcons(std::string directory) {
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

std::string HierarchyView::ResolveIcons(Entity& entity)
{
//    if (entity.GetComponent<Light>() != nullptr)
//        return "light.png";
//    else if (entity.GetComponent<Camera>() != nullptr)
//        return "camera.png";
//    else if (entity.GetComponent<MeshRenderer>() != nullptr)
//        return "object.png";

    return "object.png";
}