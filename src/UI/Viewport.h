#pragma once

#include "UI.h"
#include "PixelEngine.h"

class Viewport : public UI
{
public:
	Viewport();
	~Viewport() {
		delete _frameBuffer;
	}

	void OnGUI() override;

    Framebuffer* GetFramebuffer() const { return _frameBuffer; }

	void SetViewportSizeChangedCallback(std::function<void(const ImVec2&)> callback);
private:
	Framebuffer* _frameBuffer;

	ImVec2 _prevViewportSize;
	std::function<void(const ImVec2&)> _viewportSizeChangedCallback;
};