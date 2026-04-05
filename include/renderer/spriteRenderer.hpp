#pragma once

#include <glm/glm.hpp>

class Shader;
class Texture2D;

class SpriteRenderer
{
public:
    SpriteRenderer(Shader* shaderProg);
    ~SpriteRenderer();

    void drawSprite(Texture2D& texture, glm::vec2 pos, glm::vec2 size, 
        float rotateAngle, glm::vec3 color);
    void drawSprite(Texture2D& texture, glm::vec2 pos, glm::vec2 size, 
        float rotateAngle, glm::vec3 color, float fill, bool vertical);

private:
    Shader* shader;
    unsigned int VAO;

    void initRenderData();
};