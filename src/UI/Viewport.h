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

    void Update() override;
	void OnGUI() override;

    [[nodiscard]] Framebuffer* GetFramebuffer() const { return _frameBuffer; }

	void SetViewportSizeChangedCallback(std::function<void(const ImVec2&)> callback);
private:
	Framebuffer* _frameBuffer;

	ImVec2 _prevViewportSize;
	std::function<void(const ImVec2&)> _viewportSizeChangedCallback;

    glm::vec2 _viewportBounds[2];
};