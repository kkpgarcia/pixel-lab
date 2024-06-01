#pragma once

#include "PixelEngine.h"
#include "Assets/Project.h"

class Editor
{
public:
    static void SetProject(Project* project);
    static void SetScene(Scene* scene);
    static Scene*  GetScene() { return _scene; }
    static Project* GetProject() { return _project; }
    static bool IsProjectOpen() { return _project != nullptr; }
    static void SetCurrentSelection(UniqueID id);
    static UniqueID GetCurrentSelection() { return _currentSelection; }
private:
    static Scene* _scene;
    static Project* _project;

    //Maybe this can contain it?
    static UniqueID _currentSelection;
};