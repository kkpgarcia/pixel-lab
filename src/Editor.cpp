#include "Editor.h"

UniqueID Editor::_currentSelection = UniqueID(0);
Scene* Editor::_scene = nullptr;
Project* Editor::_project = nullptr;

void Editor::SetScene(Scene* scene)
{
	_scene = scene;
}

void Editor::SetProject(Project* project)
{
	_project = project;
}

void Editor::SetCurrentSelection(UniqueID id)
{
    auto currentScene = Editor::GetScene();

    if (currentScene == nullptr)
    {
        std::cout << "No scene open." << std::endl;
        return;
    }

    _currentSelection = id;
}

std::shared_ptr<Entity> Editor::GetCurrentSelectedEntity()
{
    auto currentScene = Editor::GetScene();

    if (currentScene == nullptr)
    {
        return nullptr;
    }

    auto entity = currentScene->Get(_currentSelection);

    if (entity == nullptr)
    {
        return nullptr;
    }

    return entity;
}