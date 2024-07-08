#pragma once

#include "PixelEngine.h"

class GeometryPass : public RenderPass
{
public:
	GeometryPass();
	void Setup() override;
	void Render(Scene& scene, Camera& camera) override;
	void Cleanup() override;

    Framebuffer* GetBuffer() const { return _gBuffer; }
private:
	Shader* _shaderGeometryPass;
	Framebuffer* _gBuffer;
};