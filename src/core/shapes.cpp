#include <core/shapes.hpp>
#include <core/shader.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cube::Cube(glm::vec3 pos, glm::vec3 col, glm::vec3 s) : position(pos), color(col), size(s)
{

    float classVertices[] = {
    // Позиции          // Нормали
        -size.x / 2.f, -size.y / 2.f, -size.z / 2.f,  0.0f,  0.0f, -1.0f,
         size.x / 2.f, -size.y / 2.f, -size.z / 2.f,  0.0f,  0.0f, -1.0f, 
         size.x / 2.f,  size.y / 2.f, -size.z / 2.f,  0.0f,  0.0f, -1.0f,
         size.x / 2.f,  size.y / 2.f, -size.z / 2.f,  0.0f,  0.0f, -1.0f,
        -size.x / 2.f,  size.y / 2.f, -size.z / 2.f,  0.0f,  0.0f, -1.0f,
        -size.x / 2.f, -size.y / 2.f, -size.z / 2.f,  0.0f,  0.0f, -1.0f,
        
        -size.x / 2.f, -size.y / 2.f,  size.z / 2.f,  0.0f,  0.0f, 1.0f,
         size.x / 2.f, -size.y / 2.f,  size.z / 2.f,  0.0f,  0.0f, 1.0f,
         size.x / 2.f,  size.y / 2.f,  size.z / 2.f,  0.0f,  0.0f, 1.0f,
         size.x / 2.f,  size.y / 2.f,  size.z / 2.f,  0.0f,  0.0f, 1.0f,
        -size.x / 2.f,  size.y / 2.f,  size.z / 2.f,  0.0f,  0.0f, 1.0f,
        -size.x / 2.f, -size.y / 2.f,  size.z / 2.f,  0.0f,  0.0f, 1.0f,
        
        -size.x / 2.f,  size.y / 2.f,  size.z / 2.f, -1.0f,  0.0f,  0.0f,
        -size.x / 2.f,  size.y / 2.f, -size.z / 2.f, -1.0f,  0.0f,  0.0f,
        -size.x / 2.f, -size.y / 2.f, -size.z / 2.f, -1.0f,  0.0f,  0.0f,
        -size.x / 2.f, -size.y / 2.f, -size.z / 2.f, -1.0f,  0.0f,  0.0f,
        -size.x / 2.f, -size.y / 2.f,  size.z / 2.f, -1.0f,  0.0f,  0.0f,
        -size.x / 2.f,  size.y / 2.f,  size.z / 2.f, -1.0f,  0.0f,  0.0f,
        
         size.x / 2.f,  size.y / 2.f,  size.z / 2.f,  1.0f,  0.0f,  0.0f,
         size.x / 2.f,  size.y / 2.f, -size.z / 2.f,  1.0f,  0.0f,  0.0f,
         size.x / 2.f, -size.y / 2.f, -size.z / 2.f,  1.0f,  0.0f,  0.0f,
         size.x / 2.f, -size.y / 2.f, -size.z / 2.f,  1.0f,  0.0f,  0.0f,
         size.x / 2.f, -size.y / 2.f,  size.z / 2.f,  1.0f,  0.0f,  0.0f,
         size.x / 2.f,  size.y / 2.f,  size.z / 2.f,  1.0f,  0.0f,  0.0f,
        
        -size.x / 2.f, -size.y / 2.f, -size.z / 2.f,  0.0f, -1.0f,  0.0f,
         size.x / 2.f, -size.y / 2.f, -size.z / 2.f,  0.0f, -1.0f,  0.0f,
         size.x / 2.f, -size.y / 2.f,  size.z / 2.f,  0.0f, -1.0f,  0.0f,
         size.x / 2.f, -size.y / 2.f,  size.z / 2.f,  0.0f, -1.0f,  0.0f,
        -size.x / 2.f, -size.y / 2.f,  size.z / 2.f,  0.0f, -1.0f,  0.0f,
        -size.x / 2.f, -size.y / 2.f, -size.z / 2.f,  0.0f, -1.0f,  0.0f,
        
        -size.x / 2.f,  size.y / 2.f, -size.z / 2.f,  0.0f,  1.0f,  0.0f,
         size.x / 2.f,  size.y / 2.f, -size.z / 2.f,  0.0f,  1.0f,  0.0f,
         size.x / 2.f,  size.y / 2.f,  size.z / 2.f,  0.0f,  1.0f,  0.0f,
         size.x / 2.f,  size.y / 2.f,  size.z / 2.f,  0.0f,  1.0f,  0.0f,
        -size.x / 2.f,  size.y / 2.f,  size.z / 2.f,  0.0f,  1.0f,  0.0f,
        -size.x / 2.f,  size.y / 2.f, -size.z / 2.f,  0.0f,  1.0f,  0.0f
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(classVertices), classVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Cube::~Cube() { }

void Cube::draw(Shader& shaderProg, bool debug)
{
    shaderProg.use();
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, position);

    shaderProg.setVec3("lightColor", color);
    shaderProg.setMat4("model", model);

    glBindVertexArray(VAO);

    if (debug)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
        glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::drawWithLight(Shader& shaderProg, bool debug)
{
    shaderProg.use();
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, position);

    shaderProg.setVec3("objColor", color);
    // shaderProg.setVec3("lightPos", lightPos);
    // glm::vec3 lightColor = glm::vec3(1.f, 1.0f, 1.0f);
    // shaderProg.setVec3("lightColor", lightColor);

    shaderProg.setMat4("model", model);

    glBindVertexArray(VAO);

    if (debug)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
        glDrawArrays(GL_TRIANGLES, 0, 36);
}


void Line::init()
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Line::updatePoints(glm::vec3 start, glm::vec3 end)
{
    vertices[0] = start;
    vertices[1] = end;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void Line::draw(glm::mat4 view, glm::mat4 proj, Shader& shader)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", proj);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

bool Line::checkCollision(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 targetPos, glm::vec3 targetSize, float& t)
{
    glm::vec3 minBound = targetPos - targetSize / 2.0f;
    glm::vec3 maxBound = targetPos + targetSize / 2.0f;

    float t1 = (minBound.x - rayOrigin.x) / rayDir.x;
    float t2 = (maxBound.x - rayOrigin.x) / rayDir.x;
    float t3 = (minBound.y - rayOrigin.y) / rayDir.y;
    float t4 = (maxBound.y - rayOrigin.y) / rayDir.y;
    float t5 = (minBound.z - rayOrigin.z) / rayDir.z;
    float t6 = (maxBound.z - rayOrigin.z) / rayDir.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax < 0 || tmin > tmax) return false;

    t = tmin;
    return true;
}

bool Line::checkCollision(glm::vec3 rayOrigin, glm::vec3 rayDir, Cube* cube, float& t) 
{
    glm::vec3 minBound = cube->position - cube->size / 2.0f;
    glm::vec3 maxBound = cube->position + cube->size / 2.0f;

    float t1 = (minBound.x - rayOrigin.x) / rayDir.x;
    float t2 = (maxBound.x - rayOrigin.x) / rayDir.x;
    float t3 = (minBound.y - rayOrigin.y) / rayDir.y;
    float t4 = (maxBound.y - rayOrigin.y) / rayDir.y;
    float t5 = (minBound.z - rayOrigin.z) / rayDir.z;
    float t6 = (maxBound.z - rayOrigin.z) / rayDir.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax < 0 || tmin > tmax) return false;

    t = tmin;
    return true;
}