#version 330 core

struct Light
{
    vec3 position;
    vec3 color;
};

uniform sampler2D u_Texture;
uniform vec3 u_ViewPos;

// Vertex In
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

// Directional Lights
uniform Light u_DirectionalLight;
uniform sampler2D u_DirectionalLightShadowMap;
uniform mat4 u_DirectionalLightMatrix;

// PointLights
uniform Light u_PointLights[1];
uniform samplerCube u_PointLightShadowMap;

// Frag Out
out vec4 FragColor;

float CalcShadowFactor(vec4 fragPosLightSpace);
float CalcPointLightShadowFactor(vec4 fragPosLightSpace);

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_DirectionalLight.color;

    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_DirectionalLight.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_DirectionalLight.color;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_DirectionalLight.color;

    // Point lights
    for (int i = 0; i < 1; ++i)
    {
        vec3 lightDir = normalize(u_PointLights[i].position - v_FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 pointDiffuse = diff * u_PointLights[i].color;

        vec3 pointReflectDir = reflect(-lightDir, norm);
        float pointSpec = pow(max(dot(viewDir, pointReflectDir), 0.0), 32.0);
        vec3 pointSpecular = specularStrength * pointSpec * u_PointLights[i].color;

        float pointShadow = CalcPointLightShadowFactor(vec4(v_FragPos, 1.0));
        pointDiffuse *= pointShadow;
        pointSpecular *= pointShadow;

        diffuse += pointDiffuse;
        specular += pointSpecular;
    }

    float shadow = CalcShadowFactor(u_DirectionalLightMatrix * vec4(v_FragPos, 1.0));
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(u_Texture, v_TexCoord).rgb;
    FragColor = vec4(lighting, 1.0);
}

float CalcShadowFactor(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_DirectionalLightShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

float CalcPointLightShadowFactor(vec4 fragPosLightSpace)
{
    vec3 fragToLight = fragPosLightSpace.xyz;
    float closestDepth = texture(u_PointLightShadowMap, fragToLight).r;
    float currentDepth = length(fragToLight);
    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}