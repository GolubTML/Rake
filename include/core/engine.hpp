#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class GLFWwindow;
class Camera;
class Shader;
class Cube;
class Player;
class Skybox;
class Line;
class Enemy;

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

    std::vector<Enemy*> enemies;
    std::vector<Line*> cross;

    float deltaTime = 0.f;
    float lastFrame = 0.f;

    float width = 800.f;
    float height = 600.f;

    Engine(glm::vec3 lightPos);
    ~Engine();

    void init();
    void run();
    void quit();

private:
    int display_w = 0, display_h = 0;

    glm::vec3 worldLight = glm::vec3(0.f);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};