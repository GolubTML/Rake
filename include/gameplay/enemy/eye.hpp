#pragma once

#include <gamecore/enemy.hpp>

class Eye : public Enemy
{
    using Enemy::Enemy;

public:
    float attackTimer = 1.f;

    void AI(float dt, Entity& player) override;
    void render(Shader& shaderProg) override;
    void renderHitbox(Shader& shader) override;
    void takeDamage(float damage, glm::vec3& knockBackDir) override;
};