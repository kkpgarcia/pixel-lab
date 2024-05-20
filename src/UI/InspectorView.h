#pragma once

#include "PixelEngine.h"
#include "UI.h"
#include "glm/glm.hpp"

struct _Transform {
	glm::vec3 position;
	glm::vec3 rotation; // Assuming rotation in degrees for easier user interaction
	glm::vec3 scale;

	_Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {} // Default constructor
};

class InspectorView : public UI
{
public:
	InspectorView() : UI("Inspector View") {}
	~InspectorView() {}

	void OnGUI() override;

private:
	_Transform transform;

	void HelpMarker(const char* desc);

    void DrawVector3Widget(const char* label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 50.0f);
};