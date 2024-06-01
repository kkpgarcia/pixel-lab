#include <filesystem>
#include "Viewport.h"
#include "Editor.h"

Viewport::Viewport() : UI("Viewport")
{
    FramebufferSpecification spec;

    spec.Width = 1280;
    spec.Height = 720;
    spec.Attachments = { { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RedInteger } };

	_frameBuffer = Framebuffer::Create(spec);
	_frameBuffer->Bind();

	OpenGLRenderbuffer* viewportRenderBuffer = new OpenGLRenderbuffer(_frameBuffer->GetWidth(), _frameBuffer->GetHeight(), TextureFormat::Depth);

	_frameBuffer->AttachRenderbuffer(viewportRenderBuffer);
	_frameBuffer->Unbind();

    _camera = new EditorCamera(1024, 1024, 45.0f, 0.1f, 1000.0f);
    _camera->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    ConstructIcons("assets/icons/toolbar");

    //Temporary container for toolbar fonts
    ImGuiIO& io = ImGui::GetIO();
    _font = io.Fonts->AddFontFromFileTTF("assets/fonts/inter/Inter-Medium.ttf", 14.0f);
}

void Viewport::ConstructIcons(std::string directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        std::string path = entry.path().string();
        std::string filename = entry.path().filename().string();
        if (filename.find(".png") != std::string::npos)
        {
            auto* texture = AssetManager::GetInstance()->Load<Texture>(path);

            _iconTextures[filename] = texture;
        }
    }
}

void Viewport::Update()
{
    _camera->ToggleInteractions(IsMouseOverWindow() && IsWindowFocused());
    _camera->OnUpdate();

    _frameBuffer->Bind();
    auto[mx,my] = ImGui::GetMousePos();
    mx -= _viewportBounds[0].x;
    my -= _viewportBounds[0].y;
    glm::vec2 viewportSize = { _viewportBounds[1] - _viewportBounds[0] };
    my = viewportSize.y - my;

    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y && !_hoveringToolbar)
    {
        auto data = _frameBuffer->ReadPixel(1, mouseX, mouseY);

        if (ImGui::IsMouseClicked(0))
        {
            auto uniqueID = UniqueID(data);
            Editor::SetCurrentSelection(uniqueID);
        }
    }
    _frameBuffer->Unbind();
}

void Viewport::OnGUI()
{
    UI::OnGUI();
	_windowSize = ImGui::GetWindowSize();
	_windowPos = ImGui::GetWindowPos();

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	if (viewportSize.x != _prevViewportSize.x || viewportSize.y != _prevViewportSize.y)
	{
		_frameBuffer->Resize(viewportSize.x, viewportSize.y);
		_prevViewportSize = viewportSize;
		if (_viewportSizeChangedCallback)
        {
			_viewportSizeChangedCallback(viewportSize);
            _camera->SetProjection(viewportSize.x, viewportSize.y, 45.0f, 0.1f, 1000.0f);
        }
	}

    auto viewportOffset = ImGui::GetCursorPos();
    auto viewportMinBound = ImGui::GetWindowPos();

    viewportMinBound.x += viewportOffset.x;
    viewportMinBound.y += viewportOffset.y;

    auto viewportMaxBound = ImVec2{ viewportMinBound.x + viewportSize.x, viewportMinBound.y + viewportSize.y };
    _viewportBounds[0] = { viewportMinBound.x, viewportMinBound.y };
    _viewportBounds[1] = { viewportMaxBound.x, viewportMaxBound.y };

	auto texture = _frameBuffer->GetColorTextures()[0]->GetID();

	ImGui::Image(reinterpret_cast<void*>(texture), viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM", ImGuiColorEditFlags_NoDragDrop))
        {
            if (payload->DataSize > 0)
            {
                std::string path = std::string((const char*)payload->Data, payload->DataSize);
                std::cout << "Dropped file: " << path << std::endl;

                std::filesystem::path p = path;
                if (p.filename().extension() == ".obj")
                {
                    auto model = AssetManager::GetInstance()->Load<Model>(path);
                    auto entity = new Entity();
                    entity->AddComponent<Model>(model);
                    Editor::GetScene()->Add(*entity);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    //Gizmos

    glm::mat4 view = glm::inverse(_camera->GetTransform().GetModelMatrix());
    glm::mat4 projection = _camera->GetProjection();

    auto currentSelection = Editor::GetCurrentSelection();
    if ((int)currentSelection != 0)
    {
        auto entity = Editor::GetScene()->Get(currentSelection);

        if (entity != nullptr)
        {
            auto transform = entity->GetComponent<Transform>();

            if (transform != nullptr)
            {
                glm::mat4 model = transform->GetModelMatrix();

                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                ImGuizmo::SetRect(viewportMinBound.x,
                                  viewportMinBound.y,
                                  viewportSize.x,
                                  viewportSize.y);

                ImGuizmo::Manipulate(glm::value_ptr(view),
                                     glm::value_ptr(projection),
                                     _currentGizmoOperation,
                                     _currentGizmoMode,
                                     glm::value_ptr(model));

                glm::vec3 gizmoPosition, gizmoRotation, gizmoScale;
                Math::DecomposeTransform(model, gizmoPosition, gizmoRotation, gizmoScale);

                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 translation, rotation, scale;
                    Math::DecomposeTransform(model, translation, rotation, scale);

                    transform->SetPosition(translation);
                    transform->SetRotation(rotation);
                    transform->SetScale(scale);
            }
            }
        }
    }

    DrawToolbar();

    // Debug
    ImGui::Begin("Engine Stats", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);// Get the size of the overlay
    ImVec2 overlaySize = ImGui::GetWindowSize();
    ImVec2 overlayPos = ImVec2(_windowPos.x + _windowSize.x - overlaySize.x, _windowPos.y + 30); // Calculate the position of the overlay

    ImGui::SetWindowPos(overlayPos);

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text("Is Toolbar: ", _hoveringToolbar);

    // End overlay
    ImGui::End();
}

void Viewport::DrawToolbar()
{
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;

    ImGui::Begin("Tools", NULL,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_AlwaysAutoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoSavedSettings);

    ImVec2 overlayPos = ImVec2(_windowPos.x + TOOLBAR_OFFSET, _windowPos.y + TOOLBAR_OFFSET); // Calculate the position of the overlay

    ImGui::SetWindowPos(overlayPos);

    auto windowSize = ImGui::GetWindowSize();
    float center_x = windowSize.x / 2;

    DrawToolbarButton("Translate", "translate.png", ImGuizmo::OPERATION::TRANSLATE, center_x);
    DrawToolbarButton("Rotate", "rotate.png", ImGuizmo::OPERATION::ROTATE, center_x);
    DrawToolbarButton("Scale", "scale.png", ImGuizmo::OPERATION::SCALE, center_x);
    DrawToolbarButton("Universal", "universal.png", ImGuizmo::OPERATION::UNIVERSAL, center_x);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    ImGui::Separator();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    ImGui::SetWindowPos(overlayPos);

    auto name = _currentGizmoMode == ImGuizmo::MODE::WORLD ? "World" : "Local";
    auto iconName = _currentGizmoMode == ImGuizmo::MODE::WORLD ? "world.png" : "local.png";

    DrawToolbarButton(name, iconName, ImGuizmo::MODE::WORLD, center_x);

    _hoveringToolbar = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
    ImGui::End();
}

void Viewport::DrawToolbarButton(const char *name, const char* iconName, ImGuizmo::OPERATION operation, float centerX)
{
    float textWidth = ImGui::CalcTextSize(name).x / 2;

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 8.0f;

    if (_currentGizmoOperation == operation)
    {
        style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    }

    if (ImGui::ImageButton(reinterpret_cast<void*>(_iconTextures[iconName]->GetID()), ImVec2(TOOLBAR_ICON_SIZE, TOOLBAR_ICON_SIZE)))
        _currentGizmoOperation = operation;

    ImGui::PushFont(_font);
    ImGui::SetCursorPosX(centerX - textWidth);
    ImGui::Text(name);
    ImGui::PopFont();
}

void Viewport::DrawToolbarButton(const char *name, const char* iconName, ImGuizmo::MODE operation, float centerX)
{
    float textWidth = ImGui::CalcTextSize(name).x / 2;

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = TOOLBAR_BUTTON_ROUNDING;

    if (_currentGizmoOperation == operation)
    {
        style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    }

    if (ImGui::ImageButton(reinterpret_cast<void*>(_iconTextures[iconName]->GetID()), ImVec2(TOOLBAR_ICON_SIZE, TOOLBAR_ICON_SIZE)))
        _currentGizmoMode = operation;

    ImGui::PushFont(_font);
    ImGui::SetCursorPosX(centerX - textWidth);
    ImGui::Text(name);
    ImGui::PopFont();
}

void Viewport::SetViewportSizeChangedCallback(std::function<void(const ImVec2&)> callback) {
	_viewportSizeChangedCallback = std::move(callback);
}

void Viewport::OnEvent(Event& e)
{
    _camera->OnEvent(e);
}