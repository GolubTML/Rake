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
protected:
    float health = 0.f;
    float hitTimer = 0.f;
    std::unique_ptr<Cube> hitbox;

public:
    glm::vec3 direction = glm::vec3(0.f); // плохо мб

    Enemy(glm::vec3 pos, glm::vec3 size, float hp);
    virtual ~Enemy() = default;

    void update(UpdateContext& ctx) override 
    { 
        AI(ctx.dt, ctx.player);

        resolveCrowding(ctx.enemies, ctx.dt);
    }
    void draw(Shader& shader) override { render(shader); }
    void drawHitbox(Shader& shader) override { renderHitbox(shader); }
    void resolveCrowding(const std::vector<std::unique_ptr<Entity>>& allEnemies, float dt);
    
    virtual void AI(float dt, Entity& player) = 0;
    virtual void render(Shader& shaderProg) = 0;
    virtual void renderHitbox(Shader& shader) = 0;
    virtual void takeDamage(float damage, glm::vec3& knockBackDir);
};