//
// Created by Kyle on 6/30/2024.
//

#include "EditorApplicationSlim.h"

#include <glm/glm.hpp>

EditorApplicationSlim::~EditorApplicationSlim()
{
    m_RenderAPI->Shutdown();
    delete m_RenderAPI;
    delete m_Camera;
    delete m_Diffuse;
}

void EditorApplicationSlim::Init()
{
    RenderAPI::SetGraphicsAPI(GraphicsAPI::OpenGL);
    if (m_RenderAPI == nullptr)
    {
        m_RenderAPI = RenderAPI::Create();
        if (m_RenderAPI == nullptr)
        {
            std::cout << "Failed to create RenderAPI!" << std::endl;
            return;
        }

    }

    m_RenderAPI->Init();

    std::string diffuseVertexShader = R"(
    #version 330 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec3 a_Normal;
    layout(location = 2) in vec2 a_TexCoord;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    out vec2 v_TexCoord;
    out vec3 v_Normal;

    void main()
    {
        v_TexCoord = a_TexCoord;
        v_Normal = a_Normal;
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    }
    )";

    std::string diffuseFragmentShader = R"(
    #version 330 core

    in vec2 v_TexCoord;
    in vec3 v_Normal;

    out vec4 FragColor;

    void main()
    {
        vec4 solidColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
        FragColor = solidColor;
    }
    )";

    m_Diffuse = new Material(Shader::Create(diffuseVertexShader.c_str(), diffuseFragmentShader.c_str()));
    //m_Shader = Shader::Create(diffuseVertexShader.c_str(), diffuseFragmentShader.c_str());
    m_Camera = new Camera();
    //
    m_Camera->GetTransform()->Translate(glm::vec3(0.0f, 0.0f, 5.0f));
}

void EditorApplicationSlim::OnUpdate()
{
    m_RenderAPI->BeginFrame();
    m_RenderAPI->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    // //
    Mesh* mesh = Mesh::Generate(Primitive::Quad);
    //
    if (mesh == nullptr)
    {
        std::cout << "Failed to generate mesh!" << std::endl;
        return;
    }

    auto model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
    auto view = m_Camera->GetView();
    auto aspectRatio = static_cast<float>(m_AppSettings.Width) / static_cast<float>(m_AppSettings.Height);
    auto projection = m_Camera->GetProjection(aspectRatio); //glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 100.0f);

    m_Diffuse->Use();

    m_Diffuse->SetMat4("u_Model", model);
    m_Diffuse->SetMat4("u_View", view);
    m_Diffuse->SetMat4("u_Projection", projection);

    m_RenderAPI->DrawIndexed(mesh, m_Diffuse->GetShader());

    delete mesh;
    m_RenderAPI->EndFrame();
}

void EditorApplicationSlim::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) { OnResizeHandler(event); return false; });
}

void EditorApplicationSlim::OnResizeHandler(WindowResizeEvent& event)
{
    Application::OnResizeHandler(event);
    m_RenderAPI->SetViewport(0, 0, event.GetWidth(), event.GetHeight());
}
