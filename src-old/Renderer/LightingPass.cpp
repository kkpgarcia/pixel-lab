#include "LightingPass.h"

LightingPass::LightingPass(GeometryPass& geometryPass) : _geometryPass(geometryPass)
{
	Shader* shaderLightingPass = AssetManager::GetInstance()->Load<Shader>("assets/shaders/DefferedShadingPass.shader");
	_shaderLightingPass = shaderLightingPass;
	_shaderLightingPass->Bind();

	_shaderLightingPass->SetInt("gPosition", 0);
	_shaderLightingPass->SetInt("gNormal", 1);
	_shaderLightingPass->SetInt("gAlbedoSpec", 2);

	_shaderLightingPass->Unbind();

	const unsigned int NR_LIGHTS = 32;
	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		// calculate slightly random offsets
		float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
		float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
		float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
		float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

	_shaderLightingPass->Bind();

	_shaderLightingPass->SetInt("gPosition", 0);
	_shaderLightingPass->SetInt("gNormal", 1);
	_shaderLightingPass->SetInt("gAlbedoSpec", 2);
}

void LightingPass::Setup()
{
	
}

void LightingPass::Render(Scene& scene, Camera& camera)
{
	OpenGLRenderer::Clear(ClearFlags::ColorBuffer | ClearFlags::DepthBuffer);
	_shaderLightingPass->Bind();

	std::vector<Texture*> textures = _geometryPass.GetBuffer()->GetColorTextures();
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i]->Bind(i);
	}

	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		_shaderLightingPass->SetVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
		_shaderLightingPass->SetVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
		// update attenuation parameters and calculate radius
		const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const float linear = 0.7f;
		const float quadratic = 1.8f;
		_shaderLightingPass->SetFloat("lights[" + std::to_string(i) + "].Linear", linear);
		_shaderLightingPass->SetFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
		// then calculate radius of light volume/sphere
		const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
		float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
		_shaderLightingPass->SetFloat("lights[" + std::to_string(i) + "].Radius", radius);
	}

	_shaderLightingPass->SetVec3("viewPos", camera.GetTransform().GetPosition());

    //we don't need to unbind this shader because it will be unbound in the next pass
}

void LightingPass::Cleanup()
{
	delete _shaderLightingPass;
}