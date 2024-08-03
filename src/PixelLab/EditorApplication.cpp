//
// Created by Kyle on 6/30/2024.
//

#include "EditorApplication.h"

#include <../../vendor/imgui/imgui.h>
#include <glm/glm.hpp>
#include <stb_image.h>


EditorApplication::~EditorApplication()
{
    m_RenderAPI->Shutdown();
    delete m_RenderAPI;
    delete m_Camera;
    delete m_Diffuse;
    delete m_Texture;
    delete m_FrameBuffer;
    delete m_Mesh;
    delete m_ScreenMesh;
    delete m_ScreenMaterial;
    delete m_Model;
    m_ModelImporter.release();

}

void EditorApplication::Init()
{
    Application::Init();
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
    out vec3 v_FragPos;

    void main()
    {
        v_TexCoord = a_TexCoord;
        v_Normal = a_Normal;
        v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    }
    )";

    std::string diffuseFragmentShader = R"(
    #version 330 core

    struct Light {
        vec3 position;
        vec3 color;
    };

    uniform sampler2D u_Texture;
    uniform Light u_DirectionalLight;
    uniform Light u_PointLights[4];
    uniform vec3 u_ViewPos;

    in vec2 v_TexCoord;
    in vec3 v_Normal;
    in vec3 v_FragPos;

    out vec4 FragColor;

    void main()
    {
        // Ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * u_DirectionalLight.color;

        // Diffuse
        vec3 norm = normalize(v_Normal);
        vec3 lightDir = normalize(u_DirectionalLight.position - v_FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * u_DirectionalLight.color;

        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(u_ViewPos - v_FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Ensure the exponent is a float
        vec3 specular = specularStrength * spec * u_DirectionalLight.color;

        for (int i = 0; i < 4; ++i)
        {
            vec3 lightDir = normalize(u_PointLights[i].position - v_FragPos);
            diff = max(dot(norm, lightDir), 0.0);
            diffuse += diff * u_PointLights[i].color;

            vec3 pointReflectDir = reflect(-lightDir, norm);
            float pointSpec = pow(max(dot(viewDir, pointReflectDir), 0.0), 32.0);
            specular += specularStrength * pointSpec * u_PointLights[i].color;
        }

        vec3 result = (ambient + diffuse + specular) * texture(u_Texture, v_TexCoord).rgb;
        FragColor = vec4(result, 1.0);
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
    auto textureData = stbi_load("assets/tests/sample_texture.png", &settings.Width, &settings.Height, &settings.Channels, 0);

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

    m_Camera = new EditorCamera();
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

    ModelImporter importer;
    m_Model = static_cast<Model*>(importer.Import("assets/tests/test1.obj"));

    m_DirectionalLight = new Entity("Driectional Light");
    auto light = m_DirectionalLight->AddComponent<DirectionalLight>();
    light->SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));

    for (int i = 0; i < 4; ++i)
    {
        auto entity = new Entity("Point Light " + std::to_string(i));
        m_PointLights[i] = entity;
        auto pointLight = entity->AddComponent<PointLight>();

        if (i == 0)
            pointLight->SetColor(glm::vec3(0.0f, 0.25f, 0.0f));
        else if (i == 1)
            pointLight->SetColor(glm::vec3(0.0f, 0.0f, 0.25f));
        else if (i == 2)
            pointLight->SetColor(glm::vec3(0.25f, 0.25f, 0.0f));
        else if (i == 3)
            pointLight->SetColor(glm::vec3(0.25f, 0.0f, 0.0f));

        float x = cos(i * 90.0f);
        float z = sin(i * 90.0f);

        auto transform = entity->GetComponent<Transform>();
        transform->Translate(glm::vec3(x * 100.0f, 0.0f, z * 100.0f));
    }

    // ImGuiStyle& style = ImGui::GetStyle();
    // style.ScaleAllSizes(2);
}

void EditorApplication::OnUpdate()
{
    m_Camera->OnUpdate();

    //Update the directional light
    auto light = m_DirectionalLight->GetComponent<DirectionalLight>();

    //Rotate the light for testing
    static float angle = 0.0f;
    angle += 1.0f * Time::GetDeltaTime();

    glm::vec3 direction = glm::vec3(cos(angle), 0.0f, sin(angle));
    light->SetDirection(direction);
}

void EditorApplication::OnRender()
{
    Application::OnRender();
    m_FrameBuffer->Bind();
    m_RenderAPI->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    m_RenderAPI->BeginFrame();

    auto model = glm::mat4(1.0f);
    auto view = m_Camera->GetView();
    auto aspectRatio = static_cast<float>(m_AppSettings.Width) / static_cast<float>(m_AppSettings.Height);
    auto projection = m_Camera->GetProjection(aspectRatio); //glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 100.0f);

    //Matrix properties
    m_Diffuse->Use();
    m_Diffuse->SetMat4("u_Model", model);
    m_Diffuse->SetMat4("u_View", view);
    m_Diffuse->SetMat4("u_Projection", projection);

    //Directional Light Properties
    auto directionalLight = m_DirectionalLight->GetComponent<DirectionalLight>();
    m_Diffuse->SetVec3("u_DirectionalLight.position", directionalLight->GetDirection());
    m_Diffuse->SetVec3("u_DirectionalLight.color", glm::vec3(1.0f, 1.0f, 1.0f));

    //Point Light Properties
    for (int i = 0; i < 4; ++i)
    {
        auto pointLight = m_PointLights[i]->GetComponent<PointLight>();
        auto transform = m_PointLights[i]->GetComponent<Transform>();
        m_Diffuse->SetVec3("u_PointLights[" + std::to_string(i) + "].position", transform->GetPosition());
        m_Diffuse->SetVec3("u_PointLights[" + std::to_string(i) + "].color", pointLight->GetColor());
    }

    //View Position
    m_Diffuse->SetVec3("u_ViewPos", m_Camera->GetTransform()->GetPosition());

    if (m_Texture != nullptr)
    {
        m_Texture->Bind();
        m_Diffuse->SetInt("u_Texture", 0);
    }

    auto meshes = m_Model->GetMeshes();

    for (auto& mesh : meshes)
    {
        m_RenderAPI->DrawIndexed(mesh, m_Diffuse->GetShader());
    }

    //m_RenderAPI->DrawIndexed(m_Mesh, m_Diffuse->GetShader());
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

void EditorApplication::OnGUI()
{
    PROFILE_SCOPE("EditorApplication::OnGUI()");

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open", "Ctrl+0")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::MenuItem("Import")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            if (ImGui::MenuItem("Asset Browser")) {}
            if (ImGui::MenuItem("Profiler"))
            {
                if (!m_ProfilerUI.IsEnabled())
                    m_ProfilerUI.Show();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    m_ProfilerUI.OnGUI();
}

void EditorApplication::OnEvent(Event& event)
{
    PROFILE_SCOPE("EditorApplication::OnEvent()");

    Application::OnEvent(event);

    m_Camera->OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyDownEvent>(BIND_EVENT(OnKeyDownHandler));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(OnResizeHandler));
}

void EditorApplication::OnResizeHandler(WindowResizeEvent& event)
{
    PROFILE_SCOPE("EditorApplication::OnResizeHandler()");

    Application::OnResizeHandler(event);

    m_RenderAPI->SetViewport(0, 0, event.GetWidth(), event.GetHeight());
    m_FrameBuffer->Resize(event.GetWidth(), event.GetHeight());
}

void EditorApplication::OnKeyDownHandler(KeyDownEvent& event)
{
    if (event.GetKey() == P)
    {
        Profiler::PrintResults();
    }
}

void EditorApplication::Shutdown()
{
    Application::Shutdown();
}
