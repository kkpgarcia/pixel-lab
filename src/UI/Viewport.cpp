#include "Viewport.h"

Viewport::Viewport() : UI("Viewport")
{
	_frameBuffer = new OpenGLFramebuffer(1280, 720);
	_frameBuffer->Bind();

	OpenGLTexture* viewportTexture = new OpenGLTexture({
		_frameBuffer->GetWidth(),
		_frameBuffer->GetHeight(),
		4,
		TextureType::None,
		ImageFormat::RGBA,
		false,
		TextureSpecification::Texture2D,
		FilterSettings{ TextureFilter::Nearest, TextureFilter::Nearest },
		WrapSettings{ TextureWrap::ClampToEdge, TextureWrap::ClampToEdge },
		DataType::UnsignedByte
		});

	OpenGLRenderbuffer* viewportRenderBuffer = new OpenGLRenderbuffer(_frameBuffer->GetWidth(), _frameBuffer->GetHeight(), ImageFormat::Depth);

	_frameBuffer->AttachColorTexture(viewportTexture);
	_frameBuffer->AttachRenderbuffer(viewportRenderBuffer);
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

	auto texture = _frameBuffer->GetColorTextures()[0]->GetID();

	ImGui::Image((void*)texture, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
}

void Viewport::SetViewportSizeChangedCallback(std::function<void(const ImVec2&)> callback) {
	_viewportSizeChangedCallback = std::move(callback);
}