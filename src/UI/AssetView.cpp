#include "AssetView.h"

void AssetView::OnGUI()
{
    ImGui::Begin("Asset Viewer");

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import New Asset...")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Search bar
    char buf[256] = "";
    ImGui::InputText("Search", buf, sizeof(buf));

    ImGui::Columns(2, NULL, false); // Two columns: sidebar and main area

    // Column 1: Asset Types sidebar
    ImGui::BeginChild("Asset Types", ImVec2(150, 0), true);
    ImGui::Text("Categories");
    ImGui::Separator();
    if (ImGui::Selectable("Textures", true)) {}
    if (ImGui::Selectable("Models")) {}
    if (ImGui::Selectable("Sounds")) {}
    ImGui::EndChild();

    ImGui::NextColumn();

    // Column 2: Asset Thumbnails
    ImGui::BeginChild("Assets", ImVec2(0, 0), true);
    ImGui::Text("Assets");
    ImGui::Separator();
    for (int i = 0; i < 10; ++i)
    {
        ImGui::Button("Asset", ImVec2(100, 100)); // Mock thumbnails
        if ((i % 3) < 2) ImGui::SameLine(); // Arrange in a grid
    }
    ImGui::EndChild();

    ImGui::End();
}