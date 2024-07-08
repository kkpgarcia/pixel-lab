#pragma once

#include "pch.h"
#include "imgui.h"
#include "glm/glm.hpp"

class LoadingWindow
{
public:
	LoadingWindow() { _instance = this; }
	~LoadingWindow() {
		delete _instance;
	}

	static LoadingWindow* GetInstance() {
		if (_instance == nullptr)
			_instance = new LoadingWindow();
		return _instance;
	}

	void Show(std::string title, glm::vec2 position);
	void Hide();
	void SetProgress(float progress);

	void Draw();

private:
	static LoadingWindow* _instance;
	std::string _title;
	float _progress;

	glm::vec2 _position;

	bool _show = false;
};