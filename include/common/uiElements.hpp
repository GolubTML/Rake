#pragma once
#include <glm/glm.hpp>
#include <string>

struct UIElement
{
    std::string name;
    std::string textureName;

    glm::vec2 pos;
    glm::vec2 size;
    glm::vec3 color;

    float rotation;
    float fillAmount;

    bool verticalFill; // по дефолту заполнение идет по горизонтали

    UIElement() = default;

    UIElement(std::string n, std::string tN, glm::vec2 pos, glm::vec2 scale);
    UIElement(std::string n, std::string tN, glm::vec2 pos, glm::vec2 scale, glm::vec3 col, 
        float rotation, float fill, bool v);
};