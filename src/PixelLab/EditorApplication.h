//
// Created by Kyle on 6/30/2024.
//

#ifndef EDITORAPPLICATIONSLIM_H
#define EDITORAPPLICATIONSLIM_H

#include "EditorCamera.h"
#include "PixelEngine.h"
#include "ProfilerUI.h"
#include "EnvironmentSettingsPanel.h"

class EditorApplication : public Application
{
public:
    EditorApplication() : Application(ApplicationSettings { "PixelLab", 1024, 720, 1000 }) { }
    ~EditorApplication() override;

    void Init() override;
    void OnUpdate() override;
    void OnRender() override;
    void OnGUI() override;
    void OnEvent(Event& event) override;
    void OnResizeHandler(WindowResizeEvent& event) override;
    void OnKeyDownHandler(KeyDownEvent& event);
    void Shutdown() override;

private:
    RenderAPI* m_RenderAPI = nullptr;

    ProfilerUI m_ProfilerUI;
    EnvironmentSettingsPanel m_EnvironmentSettingsPanel;

    //Temporary
    EditorCamera* m_Camera = nullptr;
    Material* m_Diffuse = nullptr;
    Texture* m_Texture = nullptr;
    FrameBuffer* m_FrameBuffer = nullptr;
    Mesh* m_Mesh = nullptr;
    Mesh* m_ScreenMesh = nullptr;
    Material* m_ScreenMaterial = nullptr;
    std::vector<Model*> m_Models;
    Model* m_Model = nullptr;
    Model* m_Floor = nullptr;
    std::unique_ptr<ModelImporter> m_ModelImporter;
    Entity* m_DirectionalLight = nullptr;
    Entity* m_PointLights[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    FrameBuffer* m_ShadowMap = nullptr;
    Material* m_ShadowMaterial = nullptr;
    FrameBuffer* m_PointLightFrameBuffer = nullptr;
    Material* m_PointLightMaterial = nullptr;
    Material* m_GizmoMaterial = nullptr;
    void Render(Model* model, Material* material, const glm::mat4& transform, const glm::mat4& view, const glm::mat4& projection);
};

#endif //EDITORAPPLICATIONSLIM_H
