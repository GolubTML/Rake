#pragma once
#include <common/uiElements.hpp>
#include <renderer/light.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <memory>

class GLFWwindow;
class Camera;
class Cube;
class Player;
class Skybox;
struct Line;
class Enemy;
class Mesh;
class Model;
class DebugWindow;
class ParticleGenerator;
class FontRenderer;
class SpriteRenderer;
class Projectile;
class Entity;


class Engine
{
public:
    GLFWwindow* window = nullptr;
    std::unique_ptr<DebugWindow> debugWindow;
    std::unique_ptr<Player> player;
    std::unique_ptr<FontRenderer> fontRenderer;
    std::unique_ptr<SpriteRenderer> spriteRenderer;
    std::unique_ptr<ParticleGenerator> particles;

    std::vector<std::unique_ptr<Cube>> level;
    std::unique_ptr<Line> line; // TODO: УБРАТЬ ЭТО ЕБАННОЕ ДЕРЬМО ОТ СЮДА
    std::unique_ptr<Skybox> skybox;
    
    std::vector<std::unique_ptr<Entity>> enemies;
    std::vector<std::unique_ptr<Entity>> activeProjectiles;
    
    std::map<std::string, UIElement> uiElements;
    std::vector<PointLight> lights;

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