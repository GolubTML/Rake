#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D texture_diffuse1;
uniform bool useTexture;
uniform bool isHit;

void main() 
{
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 color;
    if(useTexture) 
    {
        color = texture(texture_diffuse1, TexCoords).rgb;
    } else 
    {
        color = objectColor;
    }

    vec3 result = (ambient + diffuse) * color;

    if (isHit)
    {
        FragColor = vec4(1.f);
    }
    else
    {   
        FragColor = vec4(result, 1.0);
    }
}