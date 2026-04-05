#include <common/uiElements.hpp>

UIElement::UIElement(std::string n, std::string tN, glm::vec2 pos, glm::vec2 scale) 
    : name(n), textureName(tN), pos(pos), size(scale), color(glm::vec3(1.f)), rotation(0.f), 
    fillAmount(1.f), verticalFill(false) { }

UIElement::UIElement(std::string n, std::string tN, glm::vec2 pos, glm::vec2 scale, glm::vec3 col, 
        float rotation, float fill, bool v) 
        : name(n), textureName(tN), pos(pos), size(scale), color(col), rotation(rotation), fillAmount(fill), 
        verticalFill(v)
        { }