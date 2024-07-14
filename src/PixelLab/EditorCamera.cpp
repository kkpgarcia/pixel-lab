//
// Created by Kyle on 7/11/2024.
//

#include "EditorCamera.h"


void EditorCamera::OnUpdate()
{
    float speed = 1 * Time::GetDeltaTime();

    if (Input::OnKeyDown(KeyCode::W))
        GetTransform()->Translate(glm::vec3(0, 0, -speed));
    if (Input::OnKeyDown(KeyCode::A))
        GetTransform()->Translate(glm::vec3(-speed, 0, 0));
    if (Input::OnKeyDown(KeyCode::S))
        GetTransform()->Translate(glm::vec3(0, 0, speed));
    if (Input::OnKeyDown(KeyCode::D))
        GetTransform()->Translate(glm::vec3(speed, 0, 0));

    glm::vec2 currentMousePosition = Input::GetMousePosition();
    glm::vec2 delta = (currentMousePosition - m_LastMousePosition) * Time::GetDeltaTime();

    if (Input::OnMouseButton(Right))
        Rotate(delta);

    if (Input::OnMouseButton(Middle))
        Pan(delta);

    if (Input::OnMouseButtonUp(Right) || Input::OnMouseButtonUp(Middle))
        m_LastMousePosition = currentMousePosition;
}

void EditorCamera::Pan(const glm::vec2& delta)
{
    glm::vec3 right = GetTransform()->GetRight();
    glm::vec3 up = GetTransform()->GetUp();
    glm::vec3 translation = -delta.x * right + delta.y * up;
    GetTransform()->Translate(translation);
}

void EditorCamera::Rotate(const glm::vec2& delta)
{
    m_Yaw += delta.x;
    m_Pitch += delta.y;
    m_Pitch = glm::clamp(m_Pitch, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

    glm::quat rotation = glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));

    GetTransform()->SetRotation(rotation);
}

void EditorCamera::Zoom(MouseScrollEvent& event)
{
    float delta = event.GetY();
    float distanceChange = delta * Time::GetDeltaTime() * 50.0f;
    glm::vec3 forward = GetTransform()->GetForward();
    GetTransform()->Translate(distanceChange * forward);
}

void EditorCamera::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT(EditorCamera::Zoom));
}
