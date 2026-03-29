#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>

class Shader;

struct Character
{
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class FontRenderer
{
public:
    std::map<char, Character> characters;
    unsigned int VAO, VBO;

    FontRenderer(unsigned int width, unsigned int height);

    void load(std::string font, unsigned int fontSize);
    void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);

private:
    unsigned int screenW, screenH;
};