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


    for (int i = 0; i < _frameBuffer->GetColorTextures().size(); i++){
        auto texture = _frameBuffer->GetColorTextures()[i];

        auto formatString = texture->GetFormat() == TextureFormat::RGBA ? "RGBA" : "Red Integer";

        std::cout << "Color Attachment " << i << " ID: " << texture->GetID() << std::endl;
        std::cout << "Color Attachment " << i << " Width: " << texture->GetWidth() << std::endl;
        std::cout << "Color Attachment " << i << " Height: " << texture->GetHeight() << std::endl;
        std::cout << "Color Attachment " << i << " Format: " << formatString << std::endl;
    }
}

void Viewport::Update()
{
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
            std::cout << "Pixel Data: " << data << std::endl;

            if (data == 0)
                return;
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
			_viewportSizeChangedCallback(viewportSize);
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
                    Editor::GetInstance()->GetScene()->Add(*entity);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void Viewport::SetViewportSizeChangedCallback(std::function<void(const ImVec2&)> callback) {
	_viewportSizeChangedCallback = std::move(callback);
}