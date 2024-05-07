#version 460 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
};

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec2 GridCoords;

uniform Material material;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(Normal);

    // Set default material values
    gAlbedoSpec.rgb = texture(material.diffuse, TexCoords).rgb;
    //gAlbedoSpec.rgb = vec3(1.0);
    gAlbedoSpec.a = texture(material.specular, TexCoords).r;
}