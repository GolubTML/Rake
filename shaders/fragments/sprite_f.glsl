#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float fillAmount;
uniform bool verticalFill;
uniform bool useFill;

void main() 
{
    if (useFill)
    {
        if (verticalFill)
        {
            if (TexCoords.y > fillAmount) discard; 
        }
        else
        {
            if (TexCoords.x > fillAmount) discard; 
        }
    }

    vec4 texColor = texture(image, TexCoords);
    color = vec4(spriteColor, 1.0) * texColor;
}