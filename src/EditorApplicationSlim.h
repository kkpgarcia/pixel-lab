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

    void Init() override;
    void OnUpdate() override;

private:
    RenderAPI* m_RenderAPI = nullptr;

    //Temporary
    Camera* m_Camera = nullptr;
    Shader* m_DiffuseShader = nullptr;
};

#endif //EDITORAPPLICATIONSLIM_H
