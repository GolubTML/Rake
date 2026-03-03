#pragma once

#include <glm/glm.hpp>

class Shader;

class Cube
{
public:
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 color;

    Cube() { }
    Cube(glm::vec3 pos, glm::vec3 col, glm::vec3 s);
    ~Cube();

    void draw(Shader& shaderProg, bool debug);
    void drawWithLight(Shader& shaderProg, bool debug, glm::vec3& pos);

private:
    unsigned int VBO, VAO, EBO;
};

struct Line
{
    unsigned int VAO, VBO;
    glm::vec3 vertices[2]; // начало и конец

    void init();
    void updatePoints(glm::vec3 start, glm::vec3 end);
    void draw(glm::mat4 view, glm::mat4 proj, Shader& shader);

    bool checkCollision(glm::vec3 rayOrigin, glm::vec3 rayDir, Cube* cube, float& t);
};