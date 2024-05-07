#include "InspectorView.h"

void InspectorView::OnGUI()
{
    if (ImGui::Begin("Transform Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        // Edit position vectors
        ImGui::Text("Position");
        ImGui::SameLine(); HelpMarker("Adjust position coordinates.");
        ImGui::InputFloat3("##Position", &transform.position[0]);

        // Edit rotation vectors
        ImGui::Text("Rotation");
        ImGui::SameLine(); HelpMarker("Adjust rotation angles in degrees.");
        ImGui::InputFloat3("##Rotation", &transform.rotation[0]);

        // Edit scale vectors
        ImGui::Text("Scale");
        ImGui::SameLine(); HelpMarker("Adjust scale factors.");
        ImGui::InputFloat3("##Scale", &transform.scale[0]);

        // Optional: Reset button to reset the transform
        if (ImGui::Button("Reset Transform")) {
            transform.position = glm::vec3(0.0f);
            transform.rotation = glm::vec3(0.0f);
            transform.scale = glm::vec3(1.0f);
        }
    }
    ImGui::End();
}

void InspectorView::HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}