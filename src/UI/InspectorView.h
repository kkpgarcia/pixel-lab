#pragma once

#include "PixelEngine.h"
#include "UI.h"
#include "glm/glm.hpp"

class InspectorView : public UI
{
public:
	InspectorView() : UI("Inspector View") {}
	~InspectorView() {}

	void OnGUI() override;

private:
	void HelpMarker(const char* desc);

    void DrawVector3Widget(const char* label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 50.0f);
};