#pragma once

#include <core/projectile.hpp>

class WoodenKnife : public Projectile
{
    using Projectile::Projectile;

public:
    void AI(float dt) override;
    void render(Shader& shader) override;
    void renderHitbox(Shader& shader) override;
    void onHit(Enemy& target) override;

    bool getIsDead() override;
};