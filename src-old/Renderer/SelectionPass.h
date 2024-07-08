#pragma once

#include "PixelEngine.h"

class SelectionPass : public RenderPass
{
public:
    SelectionPass(Framebuffer& viewportFramebuffer);
    void Setup() override;
    void Render(Scene& scene, Camera& camera) override;
    void Cleanup() override;

private:
    Framebuffer& _viewportFramebuffer;
    Shader* _shaderSelectionPass;

    unsigned int _quadVAO = 0;
    unsigned int _quadVBO = 0;
};