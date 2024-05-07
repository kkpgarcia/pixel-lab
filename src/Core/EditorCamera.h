#pragma once

#include "PixelEngine.h"

class EditorCamera : public Camera
{
public:
	EditorCamera(unsigned int width, unsigned int height, float fov, float nearPlane, float farPlane)
		: Camera(width, height, fov, nearPlane, farPlane) {}
	void OnUpdate() override;
	void OnEvent(Event& e);
	void ToggleInteractions(bool enable) { _isInteracting = enable; }
private:
	float _movementSpeed = 2.0f;
	float _rotationSpeed = 1.0f;
	float _zoomSpeed = 1.0f;

	glm::vec2 _initialMousePosition = glm::vec2(0.0f);

	bool _isPanning = false;
	bool _isRotating = false;
	bool _isZooming = false;
	bool _isInteracting = true;

	void Pan(const glm::vec2& delta);
	void Rotate(const glm::vec2& delta);
	void Zoom(const float delta);

	void OnMouseScrolled(MouseScrollEvent& e);
};