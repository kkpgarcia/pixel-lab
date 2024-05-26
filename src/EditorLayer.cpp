#include "EditorLayer.h"
#include "Renderer/SelectionPass.h"

EditorLayer::EditorLayer() 
{
}

void EditorLayer::OnEnable() 
{
	_uiElements.push_back(new Menubar());
	_uiElements.push_back(new Viewport());
	//_uiElements.push_back(new Toolbar());
	_uiElements.push_back(new ContentBrowser());
	_uiElements.push_back(new HierarchyView());
	_uiElements.push_back(new InspectorView());
	
	_projectWindow = new ProjectWindow();

	Editor::GetInstance()->SetScene(new Scene());

	_geometryPass = new GeometryPass();
    Viewport* viewport = dynamic_cast<Viewport*>(GetUIElement("Viewport"));

	OpenGLRenderer::AddPass(_geometryPass);
	OpenGLRenderer::AddPass(new LightingPass(*_geometryPass));
	OpenGLRenderer::AddPass(new DrawPass(*_geometryPass->GetBuffer(), *viewport->GetFramebuffer()));
    OpenGLRenderer::AddPass(new SelectionPass(*viewport->GetFramebuffer()));

	OpenGLRenderer::Init();
	
	_camera = new EditorCamera(1024, 1024, 45.0f, 0.1f, 1000.0f);
	_camera->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    viewport->SetViewportSizeChangedCallback([&](const ImVec2& newSize) {
		_geometryPass->GetBuffer()->Resize(newSize.x, newSize.y);
		_camera->SetProjection(newSize.x, newSize.y, 45.0f, 0.1f, 1000.0f);
	});
}

void EditorLayer::OnDisable() {}
void EditorLayer::OnUpdate() 
{
    for (auto& uiElement : _uiElements)
    {
        uiElement->Update();
    }

	Viewport* viewport = dynamic_cast<Viewport*>(GetUIElement("Viewport"));
    _camera->ToggleInteractions(viewport->IsMouseOverWindow() && viewport->IsWindowFocused());
	_camera->OnUpdate();

	OpenGLRenderer::Render(*Editor::GetInstance()->GetScene(), *_camera);
}

void EditorLayer::OnGUI() 
{
	if (!Editor::GetInstance()->IsProjectOpen())
	{
		_projectWindow->OnPreRender();
		_projectWindow->OnGUI();
		_projectWindow->OnPostRender();

		return;
	}

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

	for (auto& uiElement : _uiElements)
	{
		uiElement->OnPreRender();
		uiElement->OnGUI();
		uiElement->OnPostRender();
	}

	ImGui::End();
}

void EditorLayer::OnEvent(Event& e) {
	_camera->OnEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(EditorLayer::OnWindowClose));
}

void EditorLayer::OnViewportSizeChanged(const ImVec2& size)
{
	_camera->SetProjection(size.x, size.y, 45.0f, 0.1f, 1000.0f);
	_geometryPass->GetBuffer()->Resize(size.x, size.y);
}

void EditorLayer::OnWindowClose(WindowCloseEvent& e)
{
	Application::Get()->Close();
}

UI* EditorLayer::GetUIElement(const std::string& name)
{
	for (auto& uiElement : _uiElements)
	{
		if (uiElement->GetName() == name)
			return uiElement;
	}
	return nullptr;
}