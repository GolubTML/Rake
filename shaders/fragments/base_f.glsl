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

in vec3 Normal;  
in vec3 FragPos;  
  
// uniform vec3 lightPos; 
// uniform vec3 lightColor;

uniform DirLight dirLight;

#define MAX_POINTS 10
uniform PointLight lights[MAX_POINTS];
uniform int activeLights;
uniform vec3 viewPos;
uniform vec3 objColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 ambient  = light.ambient  * objColor;
    vec3 diffuse  = light.diffuse  * diff * objColor;
    return (ambient + diffuse);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * objColor;
    vec3 diffuse  = light.diffuse  * diff * objColor;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    return (ambient + diffuse);
}

void main()
{   
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);       

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for (int i = 0; i < activeLights; ++i)
        result += CalcPointLight(lights[i], norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.f);

    // FragColor = vec4(dir, 1.0); // работает, есть какой то цвет, очень темный правда
    // FragColor = vec4(point, 1.0); // не работает, все белое
    // FragColor = vec4(vec3(float(activeLights) / 10.0), 1.0); // все серое
}