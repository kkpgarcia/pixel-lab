#pragma once

#include "PixelEngine.h"
#include "UI.h"

class HierarchyView : public UI
{
public:
	HierarchyView() : UI("Hierarchy View") {}
	~HierarchyView() {}

	void OnGUI() override;
};