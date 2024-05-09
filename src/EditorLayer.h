#pragma once

#include "PixelEngine.h"
#include "UI/Toolbar.h"
#include "UI/Menubar.h"
#include "UI/Viewport.h"
#include "Editor.h"
#include "Core/EditorCamera.h"
#include "Renderer/GeometryPass.h"
#include "Renderer/LightingPass.h"
#include "Renderer/DrawPass.h"
#include "UI/ProjectWindow.h"
#include "UI/ProjectLibrary.h"
#include "UI/HierarchyView.h"
#include "UI/InspectorView.h"

enum class SceneState
{
	Edit,
	Play
};

class EditorLayer : public Layer
{
public:
	EditorLayer();
	~EditorLayer() {
		delete _camera;
		delete _geometryPass;
		for (auto& ui : _uiElements)
			delete ui;
	}

	void OnEnable() override;
	void OnDisable() override;
	void OnGUI() override;
	void OnUpdate() override;

	void OnEvent(Event& e) override;

private:
	std::vector<UI*> _uiElements;
	EditorCamera* _camera;

	GeometryPass* _geometryPass;

	void OnViewportSizeChanged(const ImVec2& size);
	void OnWindowClose(WindowCloseEvent& e);
	UI* GetUIElement(const std::string& name);

	ProjectWindow* _projectWindow;
};