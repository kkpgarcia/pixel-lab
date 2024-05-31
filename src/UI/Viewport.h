#pragma once

#include "UI.h"
#include "PixelEngine.h"
#include "Core/EditorCamera.h"

class Viewport : public UI
{
public:
	Viewport();
	~Viewport() {
        delete _camera;
		delete _frameBuffer;
	}

    void Update() override;
	void OnGUI() override;
    void OnEvent(Event& e) override;

    [[nodiscard]] Framebuffer* GetFramebuffer() const { return _frameBuffer; }

	void SetViewportSizeChangedCallback(std::function<void(const ImVec2&)> callback);
    [[nodiscard]] EditorCamera* GetCamera() const { return _camera; }
private:
	Framebuffer* _frameBuffer;
    EditorCamera* _camera;

	ImVec2 _prevViewportSize;
	std::function<void(const ImVec2&)> _viewportSizeChangedCallback;

    glm::vec2 _viewportBounds[2];
};