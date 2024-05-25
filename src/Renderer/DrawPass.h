#pragma once

#include "PixelEngine.h"

class DrawPass : public RenderPass
{
public:
	DrawPass(Framebuffer& gBuffer, Framebuffer& drawBuffer);

	void Setup() override;
	void Render(Scene& scene, Camera& camera) override;
	void Cleanup() override;

private:
	Framebuffer& _drawBuffer;
	Framebuffer& _gBuffer;
	unsigned int _quadVAO = 0;
	unsigned int _quadVBO = 0;
};