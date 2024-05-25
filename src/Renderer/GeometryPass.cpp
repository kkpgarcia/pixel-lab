#include "GeometryPass.h"

GeometryPass::GeometryPass()
{
    Shader* shaderGeometryPass = AssetManager::GetInstance()->Load<Shader>("assets/shaders/GeometryPass.shader");
    _shaderGeometryPass = shaderGeometryPass;

    FramebufferSpecification gBufferSpec;
    gBufferSpec.Width = 1024;
    gBufferSpec.Height = 1024;
    gBufferSpec.Attachments = {
            FramebufferTextureFormat::RGBA16F,
            FramebufferTextureFormat::RGBA16F,
            FramebufferTextureFormat::RGBA8
    };

    _gBuffer = Framebuffer::Create(gBufferSpec);
    _gBuffer->Bind();

    OpenGLRenderbuffer* renderBufferDepth = new OpenGLRenderbuffer(1024, 1024, TextureFormat::Depth);

    _gBuffer->AttachRenderbuffer(renderBufferDepth);
    _gBuffer->Unbind();
}

void GeometryPass::Setup()
{
}

void GeometryPass::Render(Scene& scene, Camera& camera)
{
	_gBuffer->Bind();
	OpenGLRenderer::Clear(ClearFlags::ColorBuffer | ClearFlags::DepthBuffer);

	_shaderGeometryPass->Bind();
	_shaderGeometryPass->SetMat4("view", camera.GetView());
	_shaderGeometryPass->SetMat4("projection", camera.GetProjection());

	for (std::pair<UniqueID, std::shared_ptr<Entity>> pair : scene.GetEntities())
	{
		auto model = pair.second->GetComponent<Model>();

		if (model != nullptr)
		{
			Transform* transform = pair.second->GetComponent<Transform>();
			_shaderGeometryPass->SetMat4("model", transform->GetModelMatrix());


			for (Mesh& mesh : model->GetMeshes())
			{
				OpenGLRenderer::DrawIndexed(mesh, *_shaderGeometryPass);
			}
		}
	}

	_shaderGeometryPass->Unbind();
	_gBuffer->Unbind();
}

void GeometryPass::Cleanup()
{
	delete _shaderGeometryPass;
	delete _gBuffer;
}