//
// Created by Kyle on 6/30/2024.
//

#ifndef EDITORAPPLICATIONSLIM_H
#define EDITORAPPLICATIONSLIM_H

#include "PixelEngine-slim.h"

class EditorApplicationSlim : public Application
{
public:
    EditorApplicationSlim() : Application(ApplicationSettings { "PixelLab", 1024, 720 }) { Init(); }
    ~EditorApplicationSlim() override;

    void Init() override;
    void OnUpdate() override;
    void OnEvent(Event& event) override;
    void OnResizeHandler(WindowResizeEvent& event) override;

private:
    RenderAPI* m_RenderAPI = nullptr;

    //Temporary
    Camera* m_Camera = nullptr;
    Material* m_Diffuse = nullptr;
    Shader* m_Shader = nullptr;
};

#endif //EDITORAPPLICATIONSLIM_H
