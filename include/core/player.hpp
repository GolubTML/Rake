#pragma once
#include <glm/glm.hpp>
#include <vector>

class Cube;
class GLFWwindow;
class Camera;
class Line;
class Enemy;
class Shader;
class Model;

class Player
{
public:
    Camera* camera;

    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
    glm::vec3 size = glm::vec3(0.f);

    float speed;
    float health = 100.f;

    bool onGround;

    Player(glm::vec3 pos, glm::vec3 s, float sp, float w, float h);
    ~Player();

    void update(GLFWwindow* window, float dt, Cube* plane);
    void input(GLFWwindow* window, float dt);
    void shoot(std::vector<Enemy*> targets, Line& line);
    void resetShootTimer();
    void takeDamage(float damage);
    void drawWeapon(Shader* shader, Model* model, glm::mat4& view, glm::mat4& proj);

    bool isCollided(Cube* block);
    bool canShoot();

private:
    float gravity = 9.81f;
    float jumpPower = 5.f;

    float fireRate = 0.5f;
    float shootTimer = 0.f;
};