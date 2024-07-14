//
// Created by Kyle on 7/11/2024.
//

#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include <PixelEngine.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class EditorCamera : public Camera {
public:
    void OnUpdate();
    void OnEvent(Event& event);
private:
    glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };
    bool m_IsMouseRotationActive = false;

    float m_Yaw = 0.0f;
    float m_Pitch = 0.0f;

    void Pan(const glm::vec2& delta);
    void Rotate(const glm::vec2& delta);
    void Zoom(MouseScrollEvent& event);
};



#endif //EDITORCAMERA_H
