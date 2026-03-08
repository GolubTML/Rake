#include <core/enemy.hpp>
#include <core/shapes.hpp>
#include <core/shader.hpp>
#include <core/player.hpp>
#include <iostream>


Enemy::Enemy(glm::vec3 pos, float hp) : position(pos), health(hp)
{
    obj = new Cube(position, glm::vec3(0.5f, 0.67f, 0.5f), glm::vec3(0.5f, 1.5f, 0.5f));
}

void Enemy::update(Player* player, float dt)
{
    glm::vec3 direction = player->position - position;
    direction.y = 0.f;

    float distance = glm::length(direction);

    if (distance > 1.2f)
    {
        direction = glm::normalize(direction);

        float speed = 1.5f;
        velocity = direction * speed;

        position += velocity * dt;
    }
    else
    {
        velocity = glm::vec3(0.f);
        if (attackTimer <= 0)
        {
            player->takeDamage(10.f);
            std::cout << "PLAYER HIT! HP: " << player->health << "\n";
            attackTimer = 1.f;
        }
    }

    obj->position = position;

    if (attackTimer > 0) attackTimer -= dt;
}

void Enemy::draw(Shader* shaderProg)
{
    obj->draw(*shaderProg, false);
}

void Enemy::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0)
    {
        isDead = true;
        position.y -= 100;
        obj->position = position;
    }
}