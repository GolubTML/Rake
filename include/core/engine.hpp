#pragma once
#include <core/uiElements.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>

class GLFWwindow;
class Camera;
class Cube;
class Player;
class Skybox;
class Line;
class Enemy;
class Mesh;
class Model;
class DebugWindow;
class ParticleGenerator;
class FontRenderer;
class SpriteRenderer;
class Projectile;

struct PointLight
{
    glm::vec3 position;

    float constant = 1.f;
    float linear = 0.02f;
    float quadratic = 0.007f;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
}; // для теста пока тут

class Engine
{
public:
    GLFWwindow* window = nullptr;
    DebugWindow* debugWindow = nullptr;
    Player* player;
    FontRenderer* fontRenderer;
    SpriteRenderer* spriteRenderer;

    std::vector<Cube*> level;
    Cube* worldPlane;
    Cube* testCube;
    Line* line; // test
    Skybox* skybox;

    std::map<std::string, UIElement> uiElements;

    std::vector<Enemy*> enemies;
    std::vector<PointLight> lights;
    std::vector<Projectile*> activeProjectiles;

    ParticleGenerator* particles = nullptr;

    float deltaTime = 0.f;
    float lastFrame = 0.f;

    float width = 800.f;
    float height = 600.f;

    bool showHitboxes = false;
    bool stopEnemyAI = false;
    bool stopProjAI = false;

    Engine();
    ~Engine();

    void init();
    void run();
    void quit();

    void createEnemy();

private:
    int display_w = 0, display_h = 0;

    void input();
    void update();
    void render();

    void drawUI();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};