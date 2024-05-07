#version 460 core
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D shadowMap;
uniform samplerCube pointShadowMap;

out vec4 FragColor;
  
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform Material material;
uniform DirectionalLight dirLight;
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform float far_plane;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float CalculateShadows(vec4 fragPosLightSpace);
float CalculatePointShadows(PointLight light, vec3 fragPos);

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

void main()
{
    vec3 norm = texture(material.normal, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0) * -1;
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    vec3 result = CalculateDirectionalLight(dirLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

float CalculateShadows(vec4 fragPosLightSpace, vec3 normal)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r; 
	float currentDepth = projCoords.z;\
	float bias = max(0.05 * (1.0 - dot(normal, dirLight.direction)), 0.005);
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

     if(projCoords.z > 1.0)
        shadow = 0.0;

	return shadow;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float shadow = CalculateShadows(FragPosLightSpace, normal);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

    return lighting;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float spec = pow(max(dot(normal, lightDir), 0.0), material.shininess);

    // Calculate the distance between the fragment and the light position
    float distance = length(light.position - fragPos);

    // Calculate attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    CalculatePointShadows(light, fragPos);
    float shadow = CalculatePointShadows(light, fragPos);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

    return lighting;
}

float CalculatePointShadows(PointLight light, vec3 fragPos)
{
    vec3 fragToLight = fragPos - light.position;
    
    float closestDepth = texture(pointShadowMap, fragToLight).r;
    closestDepth *= far_plane;

    float currentDepth = length(fragToLight);

    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    
	return shadow;
}
