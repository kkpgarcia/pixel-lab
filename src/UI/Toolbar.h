#pragma once

#include "UI.h"

class Toolbar : public UI
{
public:
	Toolbar() : UI("Toolbar") {}
	~Toolbar() = default;

	void OnGUI();

	unsigned int WindowFlags() override {
		return ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_NoScrollbar;
	}
private:
	
};