#pragma once

#include "PixelEngine.h"

class GeometryPass : public RenderPass
{
public:
	GeometryPass()
	{
		ShaderAsset* shaderGeometryPass = AssetManager::GetInstance()->Load<ShaderAsset>("assets/shaders/GeometryPass.shader");
		_shaderGeometryPass = shaderGeometryPass->GetShaderData();
		_gBuffer = new OpenGLFramebuffer(1024, 1024);
		_gBuffer->Bind();


		OpenGLTexture* gPosition = new OpenGLTexture({
			_gBuffer->GetWidth(),
			_gBuffer->GetHeight(),
			4,
			TextureType::None,
			ImageFormat::RGBA16F,
			false,
			TextureSpecification::Texture2D,
			FilterSettings{ TextureFilter::Nearest, TextureFilter::Nearest },
			WrapSettings{ TextureWrap::ClampToEdge, TextureWrap::ClampToEdge },
			DataType::Float
			});

		OpenGLTexture* gNormal = new OpenGLTexture({
			_gBuffer->GetWidth(),
			_gBuffer->GetHeight(),
			4,
			TextureType::None,
			ImageFormat::RGBA16F,
			false,
			TextureSpecification::Texture2D,
			FilterSettings{ TextureFilter::Nearest, TextureFilter::Nearest },
			WrapSettings{ TextureWrap::ClampToEdge, TextureWrap::ClampToEdge },
			DataType::Float
			});

		OpenGLTexture* gAlbedoSpec = new OpenGLTexture({
			_gBuffer->GetWidth(),
			_gBuffer->GetHeight(),
			4,
			TextureType::None,
			ImageFormat::RGBA,
			false,
			TextureSpecification::Texture2D,
			FilterSettings{ TextureFilter::Nearest, TextureFilter::Nearest },
			WrapSettings{ TextureWrap::ClampToEdge, TextureWrap::ClampToEdge },
			DataType::UnsignedByte
			});

		OpenGLRenderbuffer* renderBufferDepth = new OpenGLRenderbuffer(1024, 1024, ImageFormat::Depth);

		_gBuffer->AttachColorTexture(gPosition);
		_gBuffer->AttachColorTexture(gNormal);
		_gBuffer->AttachColorTexture(gAlbedoSpec);
		_gBuffer->AttachRenderbuffer(renderBufferDepth);

		_gBuffer->Unbind();
	}
	void Setup() override;
	void Render(Scene& scene, Camera& camera) override;
	void Cleanup() override;

	OpenGLFramebuffer* GetBuffer() const { return _gBuffer; }
private:
	Shader* _shaderGeometryPass;
	OpenGLFramebuffer* _gBuffer;
};