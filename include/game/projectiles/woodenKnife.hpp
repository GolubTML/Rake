#pragma once

#include <core/projectile.hpp>

class WoodenKnife : public Projectile
{
    using Projectile::Projectile;

public:
    void AI(float dt) override;
    void draw(Shader& shader) override;
    void drawHitBox(Shader& shader) override;
    void onHit(Enemy& target, ParticleGenerator& pGen) override;

    bool isCollided(Enemy& target) override;
    bool getIsDead() override;
};