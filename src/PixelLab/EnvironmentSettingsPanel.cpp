//
// Created by Kyle on 8/21/2024.
//

#include "EnvironmentSettingsPanel.h"

#include <string>
#include <../../vendor/imgui/imgui.h>

void EnvironmentSettingsPanel::OnGUI() {
    ImGui::Begin("Environment Settings");

    ImGui::Checkbox("Directional Light", &m_DirectionalLightEnabled);
    ImGui::Checkbox("Rotate Directional Light", &m_RotateDirectionalLight);
    ImGui::SliderFloat("Directional Light Rotation", &m_DirectionalLightRotation, 0.0f, 1.0f);
    ImGui::Checkbox("Rotate Point Light", &m_RotatePointLight);
    ImGui::SliderFloat("Point Light Rotation", &m_PointLightRotation, 0.0f, 1.0f);

    ImGui::ColorEdit4("Directional Light Color", &m_DirectionalLightColor[0]);

    //ImGui::SliderInt("Point Light Count", &m_PointLightCount, 0, 10);

    for (int i = 0; i < m_PointLightCount; i++) {
        ImGui::Checkbox(("Point Light " + std::to_string(i)).c_str(), &m_PointLightColors[i].Enabled);
        ImGui::ColorEdit4(("Point Light " + std::to_string(i) + " Color").c_str(), &m_PointLightColors[i].Color[0]);
    }

    ImGui::End();
}