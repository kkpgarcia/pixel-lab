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
	    // Sensitivity factors for yaw and pitch movements
    const float yawSensitivity = 0.005f;
    const float pitchSensitivity = 0.005f;

    // Calculate the change in yaw and pitch based on mouse movement and sensitivity
    m_Yaw += delta.x * yawSensitivity;
    m_Pitch += delta.y * pitchSensitivity;

    // Clamp pitch to prevent flipping over the top
    m_Pitch = glm::clamp(m_Pitch, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

    // Convert yaw and pitch to a quaternion rotation
    glm::quat rotation = glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));


	_transform.Rotate(glm::quat(glm::vec3(-_pitch, -_yaw, 0.0f)));
}

void EditorCamera::Zoom(float delta)
{
    if (!_isInteracting) return;

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