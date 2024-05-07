#pragma once

#include "PixelEngine.h"

class UI
{
public:
	UI(const char* name) {
		_name = name;
	}

	~UI() = default;

	virtual void OnPreRender() {
		ImGui::Begin(_name, nullptr, WindowFlags());
	}
	virtual void OnGUI() {
		_windowSize = ImGui::GetWindowSize();
		_windowPos = ImGui::GetWindowPos();

		_isWindowHovered = ImGui::IsWindowHovered();
		_isWindowFocused = ImGui::IsWindowFocused();
	}

	virtual void OnPostRender() {
		ImGui::End();
	}

	unsigned int GetWindowWidth() const { return _windowSize.x; }
	unsigned int GetWindowHeight() const { return _windowSize.y; }

	virtual unsigned int WindowFlags() { return 0; }

	const char* GetName() const { return _name; }

	bool IsMouseOverWindow() const { return _isWindowHovered; }
	bool IsWindowFocused() const { return _isWindowFocused; }

	ImVec2 GetWindowSize() const { return _windowSize; }
	ImVec2 GetWindowPos() const { return _windowPos; }
protected:
	ImVec2 _windowSize;
	ImVec2 _windowPos;

	bool _isWindowHovered = false;
	bool _isWindowFocused = false;

	const char* _name;

};