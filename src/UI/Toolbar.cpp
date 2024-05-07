#include "PixelEngine.h"
#include "Toolbar.h"

void Toolbar::OnGUI()
{
	//GUI without header
	ImGui::Columns(2, "MyColumns", false); // Create 2 columns
	if (ImGui::Button("Translate"))
	{
		//m_Scene->SetGizmoType(GizmoType::Translate);
	}
	ImGui::SameLine();
	if (ImGui::Button("Rotate"))
	{
		//m_Scene->SetGizmoType(GizmoType::Rotate);
	}
	ImGui::SameLine();
	if (ImGui::Button("Scale"))
	{
		//m_Scene->SetGizmoType(GizmoType::Scale);
	}
	ImGui::SameLine();

	ImGui::NextColumn();
	if (ImGui::Button("Play"))
	{
		//m_Scene->SetState(SceneState::Play);
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause"))
	{
		//m_Scene->SetState(SceneState::Edit);
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		//m_Scene->SetState(SceneState::Edit);
		//m_Scene->Reset();
	}
}