#pragma once

#include "PixelEngine.h"
#include "Windows.h"
#include "commdlg.h"
#include "Editor.h"
#include "UI.h"
#include "Assets/Project.h"
#include "Assets/ProjectDatabase.h"

class Menubar : public UI
{
public:
	Menubar() : UI("Menubar") {}
	~Menubar() = default;
	void OnPreRender() override {}
	void OnGUI() override;
	void OnPostRender() override {}
};