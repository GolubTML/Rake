#pragma once

#include <glm/glm.hpp>
#include <common/shapes.hpp>
#include <gamecore/entity.hpp>
#include <memory>

class Shader;
class Enemy;
class ParticleGenerator;
class Cube;

class Projectile : public Entity
{
protected:
    std::unique_ptr<Cube> box;
    
    float lifeTime = 0;
    float damage = 0.f;

public:
    using SpawnCallback = std::function<void(std::unique_ptr<Projectile>)>;
    static SpawnCallback onSpawn;

    Projectile(glm::vec3 pos, glm::vec3 vel, glm::vec3 size, float lifeTime, float damage) :
        Entity(EntityType::PROJECTILE), lifeTime(lifeTime), damage(damage)
    {  
        this->position = pos;
        this->velocity = vel;
        this->size     = size;
        this->isDead   = false;

        box = std::make_unique<Cube>(pos, glm::vec3(0.f, 1.f, 0.f), size);
    }

    virtual ~Projectile() = default;

    void update(UpdateContext& ctx) override { AI(ctx.dt); }
    void draw(Shader& shader) override { render(shader); }
    void drawHitbox(Shader& shader) override { renderHitbox(shader); }

    virtual void AI(float dt) = 0;
    virtual void render(Shader& shader) = 0;
    virtual void renderHitbox(Shader& shader) = 0;
    virtual void onHit(Enemy& target) = 0;

    virtual bool getIsDead() = 0;

    template<typename T, typename... Args>
    static void NewProjectile(Args&&... args)
    {
        onSpawn(std::make_unique<T>(std::forward<Args>(args)...));
    }
};