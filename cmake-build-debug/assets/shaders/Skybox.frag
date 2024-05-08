#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform vec3 lightPos;

void main()
{    
	//vec3 fragToLight = TexCoords - lightPos;
	//float closestDepth = texture(skybox, fragToLight).r;
    //FragColor = vec4(vec3(closestDepth / 17.5), 1.0);  
	
	FragColor = texture(skybox, TexCoords);
}