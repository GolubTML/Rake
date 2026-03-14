#include <core/enemy.hpp>
#include <core/model.hpp>
#include <core/shader.hpp>
#include <core/player.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Enemy::Enemy(glm::vec3 pos, glm::vec3 size, float hp) : position(pos), size(size), health(hp)
{
}

Enemy::~Enemy()
{
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

    if (attackTimer > 0) attackTimer -= dt;
}

void Enemy::draw(Shader* shaderProg, Model* model, Player* player)
{
    if (isDead) return;

    shaderProg->use();

    glm::mat4 modelM = glm::mat4(1.f);

    modelM = glm::translate(modelM, position);

    glm::vec3 direction = player->position - position;
    float angle = atan2(direction.x, direction.z) + glm::radians(90.0f);
    modelM = glm::rotate(modelM, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    modelM = glm::scale(modelM, glm::vec3(0.3f));

    shaderProg->setMat4("model", modelM);
    model->draw(shaderProg);
}

void Enemy::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0)
    {
        isDead = true;
        position.y -= 100;
    }
}