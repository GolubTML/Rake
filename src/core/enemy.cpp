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
    if (player == nullptr) return;

    glm::vec3 direction = player->position - position;
    //direction.y = 0.f;

    float distance = glm::length(direction);

    if (distance > 0.5f)
    {
        direction = glm::normalize(direction);

        float speed = 1.5f;
        velocity = direction * speed;

        position += velocity * dt;
    }
    
    if (isCollided(*player))
    {
        
        if (attackTimer <= 0)
        {
            player->takeDamage(10.f);
            std::cout << "PLAYER HIT! HP: " << player->health << "\n";
            attackTimer = 1.f;
        }
    }
    

    if (attackTimer > 0) attackTimer -= dt;
    if (hitTimer > 0) hitTimer -= dt;
}

void Enemy::draw(Shader* shaderProg, Model* model, Player* player)
{
    if (isDead) return;

    shaderProg->use();
    shaderProg->setBool("isHit", hitTimer > 0.f);

    glm::mat4 modelM = glm::mat4(1.f);

    modelM = glm::translate(modelM, position);

    glm::vec3 direction = player->position - position;
    float angle = atan2(direction.x, direction.z) + glm::radians(90.0f);
    modelM = glm::rotate(modelM, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    float groundDist = sqrt(direction.x * direction.x + direction.z * direction.z);
    
    float pitch = atan2(direction.y, groundDist);
    modelM = glm::rotate(modelM, -pitch, glm::vec3(0.0f, 0.0f, 1.0f));

    modelM = glm::scale(modelM, glm::vec3(0.6f));

    shaderProg->setMat4("model", modelM);
    model->draw(shaderProg);

    shaderProg->setBool("isHit", false);
}

void Enemy::takeDamage(float damage, glm::vec3& knockBackDir)
{
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

bool Enemy::isCollided(Player& p)
{
    float pMinX = position.x - size.x / 2.0f;
    float pMaxX = position.x + size.x / 2.0f;
    float pMinY = position.y - size.y / 2.0f;
    float pMaxY = position.y + size.y / 2.0f;
    float pMinZ = position.z - size.z / 2.0f;
    float pMaxZ = position.z + size.z / 2.0f;

    float cMinX = p.position.x - p.size.x / 2.0f;
    float cMaxX = p.position.x + p.size.x / 2.0f;
    float cMinY = p.position.y - p.size.y / 2.0f;
    float cMaxY = p.position.y + p.size.y / 2.0f;
    float cMinZ = p.position.z - p.size.z / 2.0f;
    float cMaxZ = p.position.z + p.size.z / 2.0f;

    return (pMinX <= cMaxX && pMaxX >= cMinX) &&
           (pMinY <= cMaxY && pMaxY >= cMinY) &&
           (pMinZ <= cMaxZ && pMaxZ >= cMinZ);
}