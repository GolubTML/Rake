#version 330 core
out vec4 FragColor;

struct DirLight 
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform DirLight dirLight;
#define MAX_POINTS 10
uniform PointLight lights[MAX_POINTS];
uniform int activeLights;
uniform vec3 viewPos;

uniform vec3 objectColor;
uniform sampler2D texture_diffuse1;
uniform bool useTexture;
uniform bool isHit;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 ambient  = light.ambient  * color;
    vec3 diffuse  = light.diffuse  * diff * color;
    return (ambient + diffuse);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient * color * attenuation;
    vec3 diffuse  = light.diffuse * diff * color * attenuation;
    return (ambient + diffuse);
}

void main() 
{
    if (isHit)
    {
        FragColor = vec4(1.f);
        return;
    }
    
    vec3 color = useTexture ? texture(texture_diffuse1, TexCoords).rgb : objectColor;

    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir, color);
    for (int i = 0; i < activeLights; ++i)
        result += CalcPointLight(lights[i], norm, FragPos, viewDir, color);

    FragColor = vec4(result, 1.0);
}