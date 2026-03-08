#pragma once
#include <glm/glm.hpp>

class Cube;
class Shader;
class Player;

class Enemy
{
public:
    Cube* obj;

    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);

    float health = 0;
    float attackTimer = 1.f;
    bool isDead = false;

    Enemy(glm::vec3 pos, float hp);

    void update(Player* player, float dt);
    void draw(Shader* shaderProg);
    
    void takeDamage(float damage);
};