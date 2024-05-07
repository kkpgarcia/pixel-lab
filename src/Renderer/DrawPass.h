#pragma once

#include "PixelEngine.h"

class DrawPass : public RenderPass
{
public:
	DrawPass(OpenGLFramebuffer& gBuffer, OpenGLFramebuffer& drawBuffer);

	void Setup() override;
	void Render(Scene& scene, Camera& camera) override;
	void Cleanup() override;

private:
	OpenGLFramebuffer& _drawBuffer;
	OpenGLFramebuffer& _gBuffer;
	unsigned int _quadVAO = 0;
	unsigned int _quadVBO = 0;
};