#pragma once
#include <glm/glm.hpp>

// Самый безполезный файл - встречайте, тут будет только структура

struct PointLight
{
    glm::vec3 position;

    float constant = 1.f;
    float linear = 0.02f;
    float quadratic = 0.007f;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
