//
// Created by Kyle on 8/21/2024.
//

#ifndef ENVIRONMENTSETTINGSPANEL_H
#define ENVIRONMENTSETTINGSPANEL_H

#include <glm/glm.hpp>

struct PointLightSettings
{
    bool Enabled;
    glm::vec4 Color;
};

class EnvironmentSettingsPanel {
public:
    EnvironmentSettingsPanel() = default;
    ~EnvironmentSettingsPanel() = default;

    void OnGUI();

    bool IsDirectionalLightEnabled() const { return m_DirectionalLightEnabled; }
    bool IsPointLightEnabled() const { return m_PointLightEnabled; }
    bool IsRotateDirectionalLight() const { return m_RotateDirectionalLight; }
    float GetDirectionalLightRotation() const { return m_DirectionalLightRotation; }
    bool IsRotatePointLight() const { return m_RotatePointLight; }
    float GetPointLightRotation() const { return m_PointLightRotation; }

    int GetPointLightCount() const { return m_PointLightCount; }
    glm::vec4 GetDirectionalLightColor() const { return m_DirectionalLightColor; }
    PointLightSettings GetPointLightColor(int index) const { return m_PointLightColors[index]; }

private:
    bool m_DirectionalLightEnabled = true;
    bool m_PointLightEnabled = true;

    bool m_RotateDirectionalLight = false;
    float m_DirectionalLightRotation = 0.0f;
    bool m_RotatePointLight = false;
    float m_PointLightRotation = 0.0f;

    glm::vec4 m_DirectionalLightColor = {1.0f, 1.0f, 1.0f, 1.0f};

    int m_PointLightCount = 1;
    PointLightSettings m_PointLightColors[1] = {
    {true, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
    // {true, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
    // {true, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
    // {true, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
    // {true, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
    // {true, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
    };
};



#endif //ENVIRONMENTSETTINGSPANEL_H
