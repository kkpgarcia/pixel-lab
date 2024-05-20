#include "InspectorView.h"
#include "imgui_internal.h"

void InspectorView::OnGUI()
{
    // Edit position vectors
    ImGui::Text("Transform");
    ImGui::SameLine(); HelpMarker("Adjust position.");
    DrawVector3Widget("Position", transform.position, 0.0f, 100.0f);

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

void InspectorView::DrawVector3Widget(const char *label, glm::vec3 &values, float resetValue, float columnWidth)
{
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label);
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label);
    ImGui::NextColumn();
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.05f, 0.1f, 1.0f));

    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) values.x = resetValue;
    ImGui::PopFont();

    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));

    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
    ImGui::PopFont();

    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.3f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 0.2f, 0.7f, 1.0f));

    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) values.z = resetValue;
    ImGui::PopFont();

    ImGui::PopStyleColor(3);
    ImGui::SameLine();

    ImGui::DragFloat("##Z", &values.z, 0.1f);
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
}