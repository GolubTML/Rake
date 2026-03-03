#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWwindow;
class Camera;
class Shader;
class Cube;
class Player;
class Skybox;
class Line;

class Engine
{
public:
    GLFWwindow* window = nullptr;
    Player* player;

    Shader* shaderProg;
    Shader* rayShader;

    Cube* worldPlane;
    Line* line; // test
    Skybox* skybox;

    float deltaTime = 0.f;
    float lastFrame = 0.f;

    Engine(glm::vec3 lightPos);
    ~Engine();

    void init();
    void run();
    void quit();

    void processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp, float dt); // will be delited

private:
    glm::vec3 worldLight = glm::vec3(0.f);
};