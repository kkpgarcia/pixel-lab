#include "EditorLayer.h"

EditorLayer::EditorLayer()
{
}

void EditorLayer::OnEnable()
{
    unsigned int width = Application::Get()->GetWindow()->GetWidth();
    unsigned int height = Application::Get()->GetWindow()->GetHeight();
}

void EditorLayer::OnDisable() {}
void EditorLayer::OnUpdate()
{
}

void EditorLayer::OnGUI()
{
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/s	ttings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    // Navigation
    ImGui::Begin("Navigation");
    ImGui::Text("Navigation");
    ImGui::End();

    // Navigation
    ImGui::Begin("Inspector");
    ImGui::Text("Inspector");
    ImGui::End();

//    for (auto& uiElement : _uiElements)
//    {
//        uiElement->OnPreRender();
//        uiElement->OnGUI();
//        uiElement->OnPostRender();
//    }

    ImGui::End();
}

void EditorLayer::OnEvent(Event& e) {
//    _camera->OnEvent(e);

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(EditorLayer::OnWindowClose));
}

void EditorLayer::OnViewportSizeChanged(const ImVec2& size)
{
//    _camera->SetProjection(size.x, size.y, 45.0f, 0.1f, 1000.0f);
//    _geometryPass->GetBuffer()->Resize(size.x, size.y);
}

void EditorLayer::OnWindowClose(WindowCloseEvent& e)
{
    Application::Get()->Close();
}

//UI* EditorLayer::GetUIElement(const std::string& name)
//{
//    for (auto& uiElement : _uiElements)
//    {
//        if (uiElement->GetName() == name)
//            return uiElement;
//    }
//    return nullptr;
//}