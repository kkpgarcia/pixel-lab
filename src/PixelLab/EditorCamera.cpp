//
// Created by Kyle on 7/11/2024.
//

#include "EditorCamera.h"
#include <glm/glm.hpp>

void EditorCamera::OnUpdate()
{
    float speed = 1 * Time::GetDeltaTime();

    if (Input::IsKeyPressed(KeyCode::W))
        GetTransform()->Translate(glm::vec3(0, speed, 0));
    if (Input::IsKeyPressed(KeyCode::A))
        GetTransform()->Translate(glm::vec3(-speed, 0, 0));
    if (Input::IsKeyPressed(KeyCode::S))
        GetTransform()->Translate(glm::vec3(0, -speed, 0));
    if (Input::IsKeyPressed(KeyCode::D))
        GetTransform()->Translate(glm::vec3(speed, 0, 0));
}