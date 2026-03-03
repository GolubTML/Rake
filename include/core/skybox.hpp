#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Shader;

class Skybox
{
public:
    unsigned int VAO, VBO;
    unsigned int textureID;
    Shader* shaderProg;

    Skybox(std::vector<std::string> faces);
    ~Skybox();
    void draw(glm::mat4 view, glm::mat4 proj);

private:
    unsigned int loadCubemap(std::vector<std::string> faces);
};