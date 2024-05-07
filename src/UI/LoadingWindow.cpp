#include "LoadingWindow.h"

void LoadingWindow::Show(std::string title, glm::vec2 position)
{
	_title = title;
	_progress = 0.0f;
	_show = true;
	_position = position;
}

void LoadingWindow::Hide()
{
	_title = "";
	_progress = 0.0f;
	_show = false;
}

void LoadingWindow::SetProgress(float progress)
{
	_progress = progress;
}

void LoadingWindow::Draw()
{
	if (_show)
	{
		ImGui::Begin(_title.c_str(), &_show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
		ImGui::SetWindowPos(ImVec2(_position.x, _position.y));
		ImGui::SetWindowSize(ImVec2(400, 100));
		ImGui::Text(_title.c_str());
		ImGui::ProgressBar(_progress, ImVec2(-1.0f, 0.0f));
		ImGui::End();
	}
}
