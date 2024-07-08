#pragma once

#include "UI.h"
#include "PixelEngine.h"
#include "Core/EditorCamera.h"
#include <ImGuizmo.h>

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

    void SetGizmoOperation(ImGuizmo::OPERATION operation) { _currentGizmoOperation = operation; }
    void SetGizmoMode(ImGuizmo::MODE mode) { _currentGizmoMode = mode; }

    [[nodiscard]] ImGuizmo::OPERATION GetGizmoOperation() const { return _currentGizmoOperation; }
    [[nodiscard]] ImGuizmo::MODE GetGizmoMode() const { return _currentGizmoMode; }

    [[nodiscard]] Framebuffer* GetFramebuffer() const { return _frameBuffer; }

	void SetViewportSizeChangedCallback(std::function<void(const ImVec2&)> callback);
    [[nodiscard]] EditorCamera* GetCamera() const { return _camera; }
private:
	Framebuffer* _frameBuffer;
    EditorCamera* _camera;

	ImVec2 _prevViewportSize;
	std::function<void(const ImVec2&)> _viewportSizeChangedCallback;

    ImGuizmo::OPERATION _currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    ImGuizmo::MODE _currentGizmoMode = ImGuizmo::MODE::WORLD;

    glm::vec2 _viewportBounds[2];

    bool _hoveringToolbar = false;
    std::map<std::string, Texture*> _iconTextures;

    void ConstructIcons(std::string directory);
    void DrawToolbar();
    void DrawToolbarButton(const char* name, const char* iconName, ImGuizmo::OPERATION operation, float centerX);
    void DrawToolbarButton(const char* name, const char* iconName, ImGuizmo::MODE operation, float centerX);

    ImFont* _font;

    const float TOOLBAR_ICON_SIZE = 34.0f;
    const float TOOLBAR_OFFSET = 10.0f;
    const float TOOLBAR_BUTTON_ROUNDING = 8.0f;
};