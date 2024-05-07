#include "pch.h"
#include "EditorCamera.h"

void EditorCamera::OnUpdate()
{
	if (!_isInteracting) return;

	const glm::vec2& mouse = Input::GetMousePosition();
	glm::vec2 delta = (mouse - _initialMousePosition) * 0.003f;
	_initialMousePosition = mouse;

	if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE))
		Pan(delta);
	if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
		Rotate(delta);
}

void EditorCamera::Pan(const glm::vec2& delta)
{
	glm::vec3 right = _transform.GetRight();
	glm::vec3 up = _transform.GetUp();
	glm::vec3 translation = -delta.x * right + delta.y * up;
	_transform.Translate(translation);
}

void EditorCamera::Rotate(const glm::vec2& delta)
{
	float yawSign = _transform.GetUp().y < 0 ? -1.0f : 1.0f;
	_yaw += yawSign * delta.x * _rotationSpeed;
	_pitch += delta.y * _rotationSpeed;

	_transform.Rotate(glm::quat(glm::vec3(-_pitch, -_yaw, 0.0f)));
}

void EditorCamera::Zoom(float delta)
{
	float distanceChange = delta * _zoomSpeed;
	glm::vec3 forward = _transform.GetForward();
	_transform.Translate(distanceChange * forward);
}

void EditorCamera::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT(EditorCamera::OnMouseScrolled));
}

void EditorCamera::OnMouseScrolled(MouseScrollEvent& e)
{
	float delta = e.GetY() * 0.1f;
	Zoom(-delta);
}