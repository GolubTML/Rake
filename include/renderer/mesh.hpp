#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoord;

    Vertex() { };
    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex) 
        : position(pos), normal(norm), textureCoord(tex) {}
};

struct Texture
{
    unsigned int ID;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(Shader* shaderProg);

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};