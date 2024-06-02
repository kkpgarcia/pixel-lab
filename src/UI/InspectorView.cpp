#include "InspectorView.h"
#include "imgui_internal.h"
#include "Editor.h"

void InspectorView::OnGUI()
{
    auto selectedEntity = Editor::GetCurrentSelectedEntity();

    if (selectedEntity == nullptr)
    {
        ImGui::Text("No entity selected.");
        return;
    }

    auto transformComponent = selectedEntity->GetComponent<Transform>();

    if (transformComponent != nullptr)
    {
        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::Separator();
            DrawVector3Widget("Position", transformComponent->position, 0.0f, 100.0f);

            glm::vec3 rotation = glm::degrees(glm::eulerAngles(transformComponent->GetRotation())); // Convert quaternion to Euler angles in degrees
            DrawVector3Widget("Rotation", rotation, 0.0f, 100.0f);
            transformComponent->SetRotation(glm::quat(glm::radians(rotation)));

            DrawVector3Widget("Scale", transformComponent->scale, 1.0f, 100.0f);
        }
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
    ImGui::Separator();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponentPopup");
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