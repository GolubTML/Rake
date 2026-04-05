#pragma once
#include <glm/glm.hpp>
#include <gamecore/entity.hpp>
#include <memory>

class Model;
class Shader;
class Player;
class Cube;
class ParticleGenerator;

class Enemy : public Entity
{
public:
    std::unique_ptr<Cube> hitbox;
    glm::vec3 direction = glm::vec3(0.f); // плохо мб

    float hitTimer = 0.f;

    float health = 0;
    float attackTimer = 1.f;

    Enemy(glm::vec3 pos, glm::vec3 size, float hp);
    ~Enemy();

    void update(UpdateContext& ctx) override 
    { 
        AI(ctx.dt, ctx.player);

        resolveCrowding(ctx.enemies, ctx.dt);
    }
    void draw(Shader& shader) override { render(shader); }
    void drawHitbox(Shader& shader) override { renderHitbox(shader); }
    
    void resolveCrowding(const std::vector<std::unique_ptr<Entity>>& allEnemies, float dt);
    
    void AI(float dt, Entity& player);

    void render(Shader& shaderProg);
    void renderHitbox(Shader& shader);
    
    void takeDamage(float damage, glm::vec3& knockBackDir);

    bool isCollided(Player& p);
};