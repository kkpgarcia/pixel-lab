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

    TextImporter textImporter;

    auto diffuseVertexShader = static_cast<TextAsset*>(textImporter.Import("assets/shaders/Diffuse.vs"))->GetText();
    auto diffuseFragmentShader = static_cast<TextAsset*>(textImporter.Import("assets/shaders/Diffuse.fs"))->GetText();
    auto screenVertexShader = static_cast<TextAsset*>(textImporter.Import("assets/shaders/Screen.vs"))->GetText();
    auto screenFragmentShader = static_cast<TextAsset*>(textImporter.Import("assets/shaders/Screen.fs"))->GetText();
    auto shadwoVertexShader = static_cast<TextAsset*>(textImporter.Import("assets/shaders/Shadow.vs"))->GetText();
    auto shadowFragmentShader = static_cast<TextAsset*>(textImporter.Import("assets/shaders/Shadow.fs"))->GetText();

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
                TextureType::Texture2D,
                m_AppSettings.Width,
                m_AppSettings.Height,
                4,
                TextureFilter::Linear,
                TextureWrap::ClampToEdge,
                TextureFormat::RGBA,
                true
            }
            },
            FrameBufferAttachmentSettings {
                FrameBufferAttachmentType::Texture,
                TextureSettings {
                    TextureType::Texture2D,
                    m_AppSettings.Width,
                    m_AppSettings.Height,
                    1,
                    TextureFilter::Linear,
                    TextureWrap::ClampToEdge,
                    TextureFormat::Depth,
                    false
                }
            }
    }});

    m_ScreenMesh = Mesh::Generate(Primitive::Quad);

    ModelImporter importer;
    m_Model = static_cast<Model*>(importer.Import("assets/tests/monkey.obj"));
    m_Floor = static_cast<Model*>(importer.Import("assets/tests/room.obj"));

    m_Models.push_back(m_Model);
    m_Models.push_back(m_Floor);

    m_DirectionalLight = new Entity("Driectional Light");
    auto light = m_DirectionalLight->AddComponent<DirectionalLight>();
    m_DirectionalLight->GetComponent<Transform>()->Translate(glm::vec3(-0.0f, -10.0f, -0.0f));
    light->SetDirection(glm::vec3(0.3f, -1.0f, 0.0f));

    for (int i = 0; i < 6; ++i)
    {
        auto entity = new Entity("Point Light " + std::to_string(i));
        m_PointLights[i] = entity;
        auto pointLight = entity->AddComponent<PointLight>();
        auto pointLightSettings = m_EnvironmentSettingsPanel.GetPointLightColor(i);

        auto pointLightColor = pointLightSettings.Enabled ? pointLightSettings.Color : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        pointLight->SetColor(pointLightColor);

        float x = cos(i * 90.0f);
        float z = sin(i * 90.0f);

        auto transform = entity->GetComponent<Transform>();
        transform->Translate(glm::vec3(x * 100.0f, 0.0f, z * 100.0f));
    }

    //Shadow map
    m_ShadowMap = FrameBuffer::Create({
        1024,
        1024,
        {
            FrameBufferAttachmentSettings {
                FrameBufferAttachmentType::Texture,
                TextureSettings {
                    TextureType::Texture2D,
                    1024,
                    1024,
                    1,
                    TextureFilter::Linear,
                    TextureWrap::ClampToEdge,
                    TextureFormat::Depth,
                    false
                }
            }
        }
    });

    m_ShadowMaterial = new Material(Shader::Create(shadwoVertexShader.c_str(), shadowFragmentShader.c_str()));

    // m_PointLightFrameBuffer = FrameBuffer::Create({
    //     1024,
    //     1024,
    //     {
    //         FrameBufferAttachmentSettings {
    //             FrameBufferAttachmentType::CubeMap,
    //             TextureSettings {
    //                 TextureType::Cubemap,
    //                 1024,
    //                 1024,
    //                 1,
    //                 TextureFilter::Nearest,
    //                 TextureWrap::ClampToEdge,
    //                 TextureFormat::Depth,
    //                 false
    //             }
    //         }
    //     }
    // });

    //m_PointLightMaterial = new Material(Shader::Create(pointLightVertexShader.c_str(), pointLightFragmentShader.c_str(), pointLightGeometryShader.c_str()));

    //Debugging depth
    //m_DepthMaterial = new Material(Shader::Create(screenVertexShader.c_str(), depthFragmentShader.c_str()));

    //m_GizmoMaterial = new Material(Shader::Create(gizmoVertexShader.c_str(), gizmoFragmentShader.c_str()));

    // ImGuiStyle& style = ImGui::GetStyle();
    // style.ScaleAllSizes(2);
}

void EditorApplication::OnUpdate()
{
    m_Camera->OnUpdate();

    auto light = m_DirectionalLight->GetComponent<DirectionalLight>();
    if (m_EnvironmentSettingsPanel.IsRotateDirectionalLight())
    {
        static float angle = 0.0f;
        angle += m_EnvironmentSettingsPanel.GetDirectionalLightRotation() * Time::GetDeltaTime();

        glm::vec3 direction = glm::vec3(cos(angle), 10.0f, sin(angle));
        light->SetDirection(glm::normalize(-direction));
    }

    auto direcitonalLightColor = m_EnvironmentSettingsPanel.IsDirectionalLightEnabled() ?
        m_EnvironmentSettingsPanel.GetDirectionalLightColor() : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    light->SetColor(direcitonalLightColor);

    //Rotate all pointlights around the center point 0, 0, 0
    for (int i = 0; i < 6; ++i)
    {
        auto transform = m_PointLights[i]->GetComponent<Transform>();
        auto position = transform->GetPosition();
        static float angle = 0.0f;
        angle += i * m_EnvironmentSettingsPanel.GetPointLightRotation() * Time::GetDeltaTime();
        float x = cos(angle) * 100.0f;
        float z = sin(angle) * 100.0f;
        transform->SetPosition(glm::vec3(x, position.y, z));

        // Set color as well
        auto pointLight = m_PointLights[i]->GetComponent<PointLight>();
        auto pointLightSettings = m_EnvironmentSettingsPanel.GetPointLightColor(i);
        auto pointLightColor = pointLightSettings.Enabled ?
            pointLightSettings.Color : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        pointLight->SetColor(pointLightColor);
    }
}

void EditorApplication::OnRender()
{
    Application::OnRender();

    //Render the scene to shadow map
    m_ShadowMap->Bind();
    m_RenderAPI->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    m_RenderAPI->Clear(ClearFlags::Color | ClearFlags::Depth);
    m_RenderAPI->BeginFrame();
    m_RenderAPI->SetViewport(0, 0, 1024, 1024);

    //Shadow mapping
    auto light = m_DirectionalLight->GetComponent<DirectionalLight>();
    auto lightTransform = m_DirectionalLight->GetComponent<Transform>();

    auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 30.0f);
    auto lightView = glm::lookAt(lightTransform->GetPosition(),
                             lightTransform->GetPosition() + light->GetDirection(),
                             glm::vec3(0.0f, 1.0f, 0.0f));


    auto lightSpaceMatrix = lightProjection * lightView * lightTransform->GetModelMatrix();


    for (auto& model : m_Models)
    {
        // if (model->GetName() == "floor.obj")
        //     continue;
        m_ShadowMaterial->Use();
        m_ShadowMaterial->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);

        auto meshes = model->GetMeshes();

        for (auto& mesh : meshes)
        {
            auto transform = model->GetName() == "floor.obj" ? translate(glm::mat4(1.0), glm::vec3(0, -1.5, 0)) : glm::mat4(1.0f);
            m_ShadowMaterial->SetMat4("u_Model", transform);
            m_RenderAPI->DrawIndexed(mesh, m_ShadowMaterial->GetShader());
        }
    }

    m_RenderAPI->EndFrame();
    m_ShadowMap->Unbind();

    //Render scene to cubemap
    // m_PointLightFrameBuffer->Bind();
    // m_RenderAPI->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    // m_RenderAPI->Clear(ClearFlags::Color | ClearFlags::Depth);
    // m_RenderAPI->BeginFrame();
    // m_RenderAPI->SetViewport(0, 0, 1024, 1024);
    //
    // auto pointLightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);
    // m_PointLightMaterial->Use();
    // for (int i = 0; i < 4; ++i)
    // {
    //     auto transform = m_PointLights[i]->GetComponent<Transform>();
    //     auto lightPos = transform->GetPosition();
    //     auto pointLightView = glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    //     auto pointLightSpaceMatrix = pointLightProjection * pointLightView;
    //     m_PointLightMaterial->SetMat4("u_ShadowMatrices[" + std::to_string(i) + "]", pointLightSpaceMatrix);
    //     m_PointLightMaterial->SetMat4("u_Model", transform->GetModelMatrix());
    //     m_PointLightMaterial->SetVec3("u_LightPos", lightPos);
    // }
    //
    // m_PointLightMaterial->SetFloat("u_FarPlane", 25.0f);
    //
    // for (auto& model : m_Models)
    // {
    //     auto meshes = model->GetMeshes();
    //
    //     for (auto& mesh : meshes)
    //     {
    //         m_RenderAPI->DrawIndexed(mesh, m_PointLightMaterial->GetShader());
    //     }
    // }
    //
    // m_RenderAPI->EndFrame();
    // m_ShadowMap->Unbind();

    //Render the scene to the framebuffer
    m_FrameBuffer->Bind();
    m_RenderAPI->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    m_RenderAPI->Clear(ClearFlags::Color | ClearFlags::Depth);
    m_RenderAPI->SetViewport(0, 0, m_AppSettings.Width, m_AppSettings.Height);
    m_RenderAPI->BeginFrame();

    auto aspectRatio = static_cast<float>(m_AppSettings.Width) / static_cast<float>(m_AppSettings.Height);

    //Gizmo Drawing
    // m_GizmoMaterial->Use();
    // m_GizmoMaterial->SetMat4("u_Model",  m_DirectionalLight->GetComponent<Transform>()->GetModelMatrix());
    // m_GizmoMaterial->SetMat4("u_View", m_Camera->GetView());
    // m_GizmoMaterial->SetMat4("u_Projection", m_Camera->GetProjection(aspectRatio));
    // m_GizmoMaterial->SetVec4("u_Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    // m_DirectionalLight->GetComponent<DirectionalLight>()->DrawGizmo();

    for (auto& model : m_Models)
    {
        auto transform = model->GetName() == "floor.obj" ? translate(glm::mat4(1.0), glm::vec3(0, -1.5, 0)) : glm::mat4(1.0f);
        //Matrix properties
        m_Diffuse->Use();
        m_Diffuse->SetMat4("u_Model", transform);
        m_Diffuse->SetMat4("u_View", m_Camera->GetView());
        m_Diffuse->SetMat4("u_Projection", m_Camera->GetProjection(aspectRatio));

        //Directional Light Properties
        auto directionalLight = m_DirectionalLight->GetComponent<DirectionalLight>();
        m_Diffuse->SetVec3("u_DirectionalLight.position", directionalLight->GetDirection());
        m_Diffuse->SetVec3("u_DirectionalLight.color", directionalLight->GetColor());

        //Point Light Properties
        for (int i = 0; i < 6; ++i)
        {
            auto pointLight = m_PointLights[i]->GetComponent<PointLight>();
            auto transform = m_PointLights[i]->GetComponent<Transform>();
            m_Diffuse->SetVec3("u_PointLights[" + std::to_string(i) + "].position", transform->GetPosition());
            m_Diffuse->SetVec3("u_PointLights[" + std::to_string(i) + "].color", pointLight->GetColor());
        }

        //View Position
        m_Diffuse->SetVec3("u_ViewPos", m_Camera->GetTransform()->GetPosition());

        //Shadow map
        m_ShadowMap->GetDepthAttachment(0)->Bind(1);
        m_Diffuse->SetInt("u_ShadowMap", 1);
        m_Diffuse->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);

        if (m_Texture != nullptr)
        {
            m_Texture->Bind(0);
            m_Diffuse->SetInt("u_Texture", 0);
        }

        auto meshes = model->GetMeshes();

        for (auto& mesh : meshes)
        {
            m_RenderAPI->DrawIndexed(mesh, m_Diffuse->GetShader());
        }
    }

    m_RenderAPI->EndFrame();
    m_FrameBuffer->Unbind();

    //Render the framebuffer to screen mesh
    m_RenderAPI->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    m_RenderAPI->Clear(ClearFlags::Color | ClearFlags::Depth);
    m_RenderAPI->BeginFrame();
    m_ScreenMaterial->Use();
    m_FrameBuffer->GetColorAttachment(0)->Bind(0);
    //m_ShadowMap->GetDepthAttachment(0)->Bind(0);
    m_ScreenMaterial->SetInt("u_Texture", 0);
    m_RenderAPI->DrawIndexed(m_ScreenMesh, m_ScreenMaterial->GetShader());
    m_RenderAPI->EndFrame();
}

void EditorApplication::Render(Model* gameModel, Material* material, const glm::mat4& transform, const glm::mat4& view, const glm::mat4& projection)
{
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
    m_EnvironmentSettingsPanel.OnGUI();
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
