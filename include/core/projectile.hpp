#pragma once

#include <glm/glm.hpp>

class Shader;
class Enemy;
class ParticleGenerator;

class Projectile
{
protected:
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
    glm::vec3 hitBox   = glm::vec3(0.f);
    
    float lifeTime = 0;
    float damage = 0.f;
    bool isDead = false;

public:

    Projectile(glm::vec3 pos, glm::vec3 vel, glm::vec3 size, float lifeTime, float damage) : position(pos), velocity(vel), hitBox(size),
        lifeTime(lifeTime), damage(damage), isDead(false) { }
    virtual ~Projectile() = default;

    virtual void AI(float dt) = 0;
    virtual void draw(Shader& shader) = 0;
    virtual void onHit(Enemy& target, ParticleGenerator& pGen) = 0;

    virtual bool isCollided(Enemy& target) = 0;
    virtual bool getIsDead() = 0;
};