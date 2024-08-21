#version 330 core

in vec4 FragPos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

void main()
{
    float lightDistance = length(FragPos.xyz - u_LightPos);
    float lightIntensity = lightDistance / u_FarPlane;
    gl_FragColor = vec4(vec3(lightIntensity), 1.0);
}