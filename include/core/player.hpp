#pragma once
#include <glm/glm.hpp>

class Cube;
class GLFWwindow;
class Camera;
class Line;

class Player
{
public:
    Camera* camera;

    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
    glm::vec3 size = glm::vec3(0.f);

    float speed;

    bool onGround;

    Player(glm::vec3 pos, glm::vec3 s, float sp);
    ~Player();

    void update(GLFWwindow* window, float dt, Cube* plane);
    void input(GLFWwindow* window, float dt);
    void shoot(Cube* target, Line& line);

    bool isCollided(Cube* block);

private:
    float gravity = 9.81f;
    float jumpPower = 5.f;
};