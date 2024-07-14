//
// Created by Kyle on 6/30/2024.
//

#ifndef EDITORAPPLICATIONSLIM_H
#define EDITORAPPLICATIONSLIM_H

#include "EditorCamera.h"
#include "PixelEngine.h"
#include "ProfilerUI.h"

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

    //Temporary
    EditorCamera* m_Camera = nullptr;
    Material* m_Diffuse = nullptr;
    Texture* m_Texture = nullptr;
    FrameBuffer* m_FrameBuffer = nullptr;
    Mesh* m_Mesh = nullptr;
    Mesh* m_ScreenMesh = nullptr;
    Material* m_ScreenMaterial = nullptr;
    Model* m_Model = nullptr;
    std::unique_ptr<ModelImporter> m_ModelImporter;
};

#endif //EDITORAPPLICATIONSLIM_H
