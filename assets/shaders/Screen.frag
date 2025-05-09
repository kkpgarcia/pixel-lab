#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TexCoord;
in vec3 v_Normal;

out vec4 FragColor;

void main()
{
    FragColor = texture(u_Texture, v_TexCoord);
}