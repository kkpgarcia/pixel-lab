//
// Created by Kyle on 6/30/2024.
//

#include "EditorApplicationSlim.h"

#include <glm/glm.hpp>

#include <stb_image.h>

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

    uniform sampler2D u_Texture;

    in vec2 v_TexCoord;
    in vec3 v_Normal;

    out vec4 FragColor;

    void main()
    {
        FragColor = texture(u_Texture, v_TexCoord);
    }
    )";

    std::string screenVertexShader = R"(
    #version 330 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec3 a_Normal;
    layout(location = 2) in vec2 a_TexCoord;

    out vec2 v_TexCoord;
    out vec3 v_Normal;

    void main()
    {
        v_TexCoord = a_TexCoord;
        v_Normal = a_Normal;
        gl_Position = vec4(a_Position, 1.0);
    }
    )";

    std::string screenFragmentShader = R"(
    #version 330 core

    uniform sampler2D u_Texture;

    in vec2 v_TexCoord;
    in vec3 v_Normal;

    out vec4 FragColor;

    void main()
    {
        FragColor = texture(u_Texture, v_TexCoord);
    }
    )";

    TextureSettings settings;

    stbi_set_flip_vertically_on_load(true);
    auto textureData = stbi_load("assets/sample_texture.png", &settings.Width, &settings.Height, &settings.Channels, 0);

    if (textureData == nullptr)
    {
        std::cout << "Failed to load texture data!" << std::endl;
    }
    else
    {
        settings.Format = settings.Channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
        settings.GenerateMipmaps = true;
        m_Texture = Texture::Create(settings, textureData);
    }

    delete textureData;

    m_Diffuse = new Material(Shader::Create(diffuseVertexShader.c_str(), diffuseFragmentShader.c_str()));
    m_ScreenMaterial = new Material(Shader::Create(screenVertexShader.c_str(), screenFragmentShader.c_str()));

    m_Camera = new Camera();
    m_Camera->GetTransform()->Translate(glm::vec3(0.0f, 0.0f, 5.0f));

    m_FrameBuffer = FrameBuffer::Create({
        m_AppSettings.Width,
        m_AppSettings.Height,
{
        FrameBufferAttachmentSettings {
            FrameBufferAttachmentType::Texture,
            TextureSettings {
                m_AppSettings.Width,
                m_AppSettings.Height,
                4,
                TextureFilter::Linear,
                TextureWrap::ClampToEdge,
                TextureFormat::RGBA,
                true
            }
        }
    }});

    m_ScreenMesh = Mesh::Generate(Primitive::Quad);
    m_Mesh = Mesh::Generate(Primitive::Quad);
}

void EditorApplicationSlim::OnUpdate()
{
    m_FrameBuffer->Bind();
    m_RenderAPI->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    m_RenderAPI->BeginFrame();

    auto model = glm::mat4(1.0f);
    auto view = m_Camera->GetView();
    auto aspectRatio = static_cast<float>(m_AppSettings.Width) / static_cast<float>(m_AppSettings.Height);
    auto projection = m_Camera->GetProjection(aspectRatio); //glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 100.0f);


    m_Diffuse->Use();
    m_Diffuse->SetMat4("u_Model", model);
    m_Diffuse->SetMat4("u_View", view);
    m_Diffuse->SetMat4("u_Projection", projection);

    if (m_Texture != nullptr)
    {
        m_Texture->Bind();
        m_Diffuse->SetInt("u_Texture", 0);
    }

    m_RenderAPI->DrawIndexed(m_Mesh, m_Diffuse->GetShader());
    m_RenderAPI->EndFrame();

    m_FrameBuffer->Unbind();

    //Render the framebuffer to screen mesh
    m_RenderAPI->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    m_RenderAPI->BeginFrame();
    m_ScreenMaterial->Use();
    m_FrameBuffer->GetColorAttachment(0)->Bind();
    m_ScreenMaterial->SetInt("u_Texture", 0);
    m_RenderAPI->DrawIndexed(m_ScreenMesh, m_ScreenMaterial->GetShader());
    m_RenderAPI->EndFrame();
}

void EditorApplicationSlim::OnEvent(Event& event)
{
    Application::OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyDownEvent>(BIND_EVENT(OnKeyDownHandler));
}

void EditorApplicationSlim::OnResizeHandler(WindowResizeEvent& event)
{
    Application::OnResizeHandler(event);
    m_RenderAPI->SetViewport(0, 0, event.GetWidth(), event.GetHeight());
    m_FrameBuffer->Resize(m_AppSettings.Width, m_AppSettings.Height);
}

void EditorApplicationSlim::OnKeyDownHandler(KeyDownEvent& event)
{
    if (event.GetKey() == P)
    {
        Profiler::PrintResults();
    }
}
