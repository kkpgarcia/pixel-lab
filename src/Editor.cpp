#include "Editor.h"

Editor Editor::_instance;

void Editor::SetScene(Scene* scene)
{
	_scene = scene;
}

void Editor::SetProject(Project* project)
{
	_project = project;
}