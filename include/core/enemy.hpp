#pragma once
#include <glm/glm.hpp>

class Model;
class Shader;
class Player;
class Cube;
class ParticleGenerator;

class Enemy
{
public:
    Cube* hitbox = nullptr;
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
    glm::vec3 size = glm::vec3(0.f);

    float hitTimer = 0.f;

    float health = 0;
    float attackTimer = 1.f;
    bool isDead = false;

    Enemy(glm::vec3 pos, glm::vec3 size, float hp);
    ~Enemy();

    void update(Player* player, float dt);
    void resolveCrowding(std::vector<Enemy*>& allEnemies, float dt);

    void draw(Shader* shaderProg, Model* model, Player* player);
    void drawHitbox(Shader* shader);
    
    void takeDamage(float damage, glm::vec3& knockBackDir, ParticleGenerator& pGen);

    bool isCollided(Player& p);
};