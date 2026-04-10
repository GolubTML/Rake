#include <gameplay/enemy/eye.hpp>
#include <core/assetManager.hpp>
#include <renderer/shader.hpp>
#include <renderer/model.hpp>
#include <common/shapes.hpp>
#include <gamecore/player.hpp>
#include <renderer/particles.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Eye::AI(float dt, Entity& player)
{
    direction = player.position - position;

    float distance = glm::length(direction);

    if (distance > 0.5f)
    {
        direction = glm::normalize(direction);

        float speed = 1.5f;
        velocity = direction * speed;

        position += velocity * dt;
    }
    
    if (checkCollision(player))
    {
        
        if (attackTimer <= 0)
        {
            Player* p = dynamic_cast<Player*>(&player);

            if (p)
            {
                p->takeDamage(10.f);
                attackTimer = 1.f;
            } 
        }
    }
    
    hitbox->position = position;

    angle = atan2(direction.x, direction.z) + glm::radians(90.0f);

    if (attackTimer > 0) attackTimer -= dt;
    if (hitTimer > 0) hitTimer -= dt;
}

void Eye::render(Shader& shaderProg)
{
    if (isDead) return;

    shaderProg.use();
    shaderProg.setBool("isHit", hitTimer > 0.f);

    Model& model = AssetManager::getModel("eye");

    glm::mat4 modelM = glm::mat4(1.f);

    modelM = glm::translate(modelM, position);

    modelM = glm::rotate(modelM, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    float groundDist = sqrt(direction.x * direction.x + direction.z * direction.z);
    
    float pitch = atan2(direction.y, groundDist);
    modelM = glm::rotate(modelM, -pitch, glm::vec3(0.0f, 0.0f, 1.0f));

    modelM = glm::scale(modelM, glm::vec3(0.6f));

    shaderProg.setMat4("model", modelM);
    model.draw(&shaderProg);

    shaderProg.setBool("isHit", false);
}

void Eye::renderHitbox(Shader& shader)
{
    if (isDead) return;

    hitbox->drawWithLight(shader, true);
}

void Eye::takeDamage(float damage, glm::vec3& knockBackDir)
{
    Enemy::takeDamage(damage, knockBackDir);
}