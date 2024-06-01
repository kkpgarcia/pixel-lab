#include <filesystem>
#include "Viewport.h"
#include "Editor.h"
#include <ImGuizmo.h>

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


    for (int i = 0; i < _frameBuffer->GetColorTextures().size(); i++){
        auto texture = _frameBuffer->GetColorTextures()[i];

        auto formatString = texture->GetFormat() == TextureFormat::RGBA ? "RGBA" : "Red Integer";

        std::cout << "Color Attachment " << i << " ID: " << texture->GetID() << std::endl;
        std::cout << "Color Attachment " << i << " Width: " << texture->GetWidth() << std::endl;
        std::cout << "Color Attachment " << i << " Height: " << texture->GetHeight() << std::endl;
        std::cout << "Color Attachment " << i << " Format: " << formatString << std::endl;
    }

    _camera = new EditorCamera(1024, 1024, 45.0f, 0.1f, 1000.0f);
    _camera->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
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

    if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
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
                                     ImGuizmo::OPERATION::TRANSLATE,
                                     ImGuizmo::MODE::LOCAL,
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

    // Debug
//    ImGui::Begin("Engine Stats", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);// Get the size of the overlay
//    ImVec2 overlaySize = ImGui::GetWindowSize();
//    ImVec2 overlayPos = ImVec2(_windowPos.x + _windowSize.x - overlaySize.x, _windowPos.y + 30); // Calculate the position of the overlay
//
//    ImGui::SetWindowPos(overlayPos);
//
//    ImGui::Text("Viewport Postion: %f, %f", _windowPos.x, _windowPos.y);
//    ImGui::Text("Viewport Size: %f x %f", _windowSize.x, _windowSize.y);
//    ImGui::Text("Camera Position: %f, %f, %f", _camera->GetTransform().GetPosition().x, _camera->GetTransform().GetPosition().y, _camera->GetTransform().GetPosition().z);
//    ImGui::Text("Is Mouse Over: %d", IsMouseOverWindow());
//    ImGui::Text("Is Window Focused: %d", IsWindowFocused());
//    ImGui::Text("Mouse Position: %f, %f", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
//    ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo":"Not over gizmo");
//    ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "Not Interacting with translate gizmo");
//
//    // End overlay
//    ImGui::End();

    
}

void Viewport::SetViewportSizeChangedCallback(std::function<void(const ImVec2&)> callback) {
	_viewportSizeChangedCallback = std::move(callback);
}

void Viewport::OnEvent(Event& e)
{
    _camera->OnEvent(e);
}