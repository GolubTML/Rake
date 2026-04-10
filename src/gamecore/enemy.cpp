#include <gamecore/enemy.hpp>
#include <renderer/model.hpp>
#include <renderer/shader.hpp>
#include <gamecore/player.hpp>
#include <common/shapes.hpp>
#include <renderer/particles.hpp>
#include <core/assetManager.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Enemy::Enemy(glm::vec3 pos, glm::vec3 size, float hp) : Entity(EntityType::ENEMY), health(hp)
{
    this->position = pos;
    this->velocity = glm::vec3(0.f);
    this->size     = size;
    this->isDead   = false;

    hitbox = std::make_unique<Cube>(position, glm::vec3(0.f, 1.f, 0.f), size);
}

void Enemy::resolveCrowding(const std::vector<std::unique_ptr<Entity>>& allEnemies, float dt)
{
    for (auto& other: allEnemies)
    {
        if (other.get() == this) continue;

        float dist = glm::distance(this->position, other->position);
        float minDist = (this->size.x + other->size.x) * 0.8f;

        if (dist < minDist && dist > 0.001f)
        {
            glm::vec3 pushDir = glm::vec3(this->position - other->position);
            float force = (minDist - dist) / minDist;

            this->position += pushDir * force * 2.0f * dt;
        }
    }
}

void Enemy::takeDamage(float damage, glm::vec3& knockBackDir) 
{
    for (int i = 0; i <= 20; ++i)
    {
        float vx = ((rand() % 100) - 50) / 10.f;
        float vy = ((rand() % 100) - 50) / 10.f;
        float vz = ((rand() % 100) - 50) / 10.f;
        glm::vec3 randVel(vx, vy, vz);

        ParticleGenerator::emit(position, randVel, glm::vec4(0.8f, 0.f, 0.f, 1.f));
    }

    hitTimer = 0.2f;
    health -= damage;

    float force = 0.5f;
    position += knockBackDir * force;
    if (health <= 0)
    {
        isDead = true;
        position.y -= 100;
    }
}