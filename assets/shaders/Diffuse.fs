#version 330 core

struct Light {
    vec3 position;
    vec3 color;
};

uniform sampler2D u_Texture;
uniform Light u_DirectionalLight;
uniform Light u_PointLights[6];
uniform vec3 u_ViewPos;

uniform sampler2D u_ShadowMap;
uniform mat4 u_LightSpaceMatrix;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 FragColor;

float CalcShadowFactor(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_DirectionalLight.color;

    // Diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_DirectionalLight.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_DirectionalLight.color;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Ensure the exponent is a float
    vec3 specular = specularStrength * spec * u_DirectionalLight.color;

    for (int i = 0; i < 6; ++i)
    {
        vec3 lightDir = normalize(u_PointLights[i].position - v_FragPos);
        diff = max(dot(norm, lightDir), 0.0);
        diffuse += diff * u_PointLights[i].color;

        vec3 pointReflectDir = reflect(-lightDir, norm);
        float pointSpec = pow(max(dot(viewDir, pointReflectDir), 0.0), 32.0);
        specular += specularStrength * pointSpec * u_PointLights[i].color;
    }

    //Calculate shadow factor
    vec4 fragPosLightSpace = u_LightSpaceMatrix * vec4(v_FragPos, 1.0);
    float shadow = CalcShadowFactor(fragPosLightSpace);

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(u_Texture, v_TexCoord).rgb;
    FragColor = vec4(result, 1.0);
}