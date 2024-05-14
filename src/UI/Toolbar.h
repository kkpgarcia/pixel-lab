#pragma once

#include "UI.h"

class Toolbar : public UI
{
public:
	Toolbar() : UI("Toolbar") {}
	~Toolbar() = default;

    void OnPreRender() override {}
	void OnGUI() override;
    void OnPostRender() override {}

private:
	unsigned int WindowFlags() override {
		return ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_NoScrollbar;
	}

};