#include "SelectionPass.h"

SelectionPass::SelectionPass(Framebuffer& viewportBuffer)
    : _viewportFramebuffer(viewportBuffer)
{
    _shaderSelectionPass = AssetManager::GetInstance()->Load<Shader>("assets/shaders/SelectionShader.shader");
}

void SelectionPass::Setup()
{
}

void SelectionPass::Render(Scene &scene, Camera &camera)
{
    _viewportFramebuffer.Bind();
    GLenum drawBuffer = GL_COLOR_ATTACHMENT1;
    glDrawBuffers(1, &drawBuffer);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //OpenGLRenderer::Clear(ClearFlags::ColorBuffer | ClearFlags::DepthBuffer);

    _shaderSelectionPass->Bind();
    _shaderSelectionPass->SetMat4("view", camera.GetView());
    _shaderSelectionPass->SetMat4("projection", camera.GetProjection());

    for (std::pair<UniqueID, std::shared_ptr<Entity>> pair : scene.GetEntities())
    {
        auto model = pair.second->GetComponent<Model>();

        if (model != nullptr)
        {
            Transform* transform = pair.second->GetComponent<Transform>();
            _shaderSelectionPass->SetMat4("model", transform->GetModelMatrix());
            _shaderSelectionPass->SetInt("objectID", pair.first.ToInt());

            for (Mesh& mesh : model->GetMeshes())
            {
                OpenGLRenderer::DrawIndexed(mesh, *_shaderSelectionPass);
            }
        }
    }

    _shaderSelectionPass->Unbind();
    _viewportFramebuffer.Unbind();
}

void SelectionPass::Cleanup()
{
}