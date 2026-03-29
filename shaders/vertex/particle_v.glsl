#version 330 core
layout (location = 0) in vec3 aPos; 

out float ParticleLife; 

uniform mat4 model; 
uniform mat4 view;  
uniform mat4 proj;  

uniform float life; 
uniform vec4 particleColor; 

void main()
{
    ParticleLife = life; 
    
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}