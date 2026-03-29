#version 330 core
out vec4 FragColor;

uniform vec4 color;
uniform float life;

void main()
{
    FragColor = vec4(color);
}