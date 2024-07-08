#pragma once

#include "PixelEngine.h"
#include "GeometryPass.h"

class LightingPass : public RenderPass
{
public:
	LightingPass(GeometryPass& geometryPass);
	~LightingPass() = default;
	void Setup() override;
	void Render(Scene& scene, Camera& camera) override;
	void Cleanup() override;

private:
	GeometryPass& _geometryPass;
	Shader* _shaderLightingPass;

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
};