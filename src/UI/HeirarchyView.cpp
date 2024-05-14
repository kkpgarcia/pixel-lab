#include "HierarchyView.h"

void HierarchyView::OnGUI()
{
    // Root of the hierarchy or scene
    if (ImGui::TreeNode("Root Node"))
    {
        // Child nodes
        if (ImGui::TreeNode("Child Node 1"))
        {
            ImGui::Text("Object properties here.");
            ImGui::TreePop(); // Don't forget to pop!
        }
        if (ImGui::TreeNode("Child Node 2"))
        {
            if (ImGui::TreeNode("Child Node 2a"))
            {
                ImGui::Text("More nested items here.");
                ImGui::TreePop(); // Always pair TreePush/TreePop
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}