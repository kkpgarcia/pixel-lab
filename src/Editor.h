#pragma once

#include "PixelEngine.h"
#include "Assets/Project.h"

class Editor
{
public:
	static Editor* GetInstance() { return &_instance; }
	void SetProject(Project* project);
	void SetScene(Scene* scene);
	Scene* GetScene() { return _scene; }
	Project* GetProject() { return _project; }
	bool IsProjectOpen() { return _project != nullptr; }
private:
	static Editor _instance;
	Scene* _scene;
	Project* _project;
};