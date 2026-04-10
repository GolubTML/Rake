#version 330 core
out vec4 FragColor;
in vec3 TexCoords; 

uniform samplerCube skybox; 
uniform float time;

void main()
{    
    vec3 d = normalize(TexCoords);

    d.x += sin(d.y * 10.0 + time) * 0.05;
    d.z += cos(d.x * 10.0 + time) * 0.05;

    vec3 color = texture(skybox, normalize(d)).rgb;

    color *= vec3(0.2, 0.05, 0.0);

    float flicker = 0.5 + 0.05 * sin(time * 20.0); 
    color *= flicker;
    FragColor = vec4(color, 1.0);
}