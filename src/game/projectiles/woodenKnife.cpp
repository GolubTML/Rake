#include <game/projectiles/woodenKnife.hpp>
#include <core/assetManager.hpp>
#include <core/shader.hpp>
#include <core/model.hpp>
#include <core/enemy.hpp>

#include <glm/gtc/matrix_transform.hpp>

void WoodenKnife::AI(float dt) 
{
    position += velocity * dt;


    box->position = position;
    lifeTime -= dt;
    if (lifeTime < 0.f) isDead = true;
}

void WoodenKnife::draw(Shader& shader) 
{
    if (isDead) return;

    Model& model = AssetManager::getModel("woodenKnife");

    glm::mat4 modelM = glm::mat4(1.f);
    modelM = glm::translate(modelM, position);

    if (velocity.length() > 0.01f)
    {
        glm::vec3 direction = glm::normalize(velocity);
        glm::vec3 lookAtTarget = position + direction;
        
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        if (std::abs(glm::dot(direction, worldUp)) > 0.99f) 
            worldUp = glm::vec3(1.0f, 0.0f, 0.0f);

        glm::mat4 lookAtMat = glm::lookAt(position, lookAtTarget, worldUp);
        glm::mat4 rotationMat = glm::inverse(lookAtMat);
        
        rotationMat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        modelM *= rotationMat;
    }

    modelM = glm::rotate(modelM, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelM = glm::scale(modelM, glm::vec3(0.3f));

    shader.setMat4("model", modelM);
    
    model.draw(&shader);
}

void WoodenKnife::drawHitBox(Shader& shader)
{
    if (isDead) return;

    box->drawWithLight(shader, true);
}

void WoodenKnife::onHit(Enemy& target, ParticleGenerator& pGen) 
{
    glm::vec3 dir = glm::normalize(velocity);
    target.takeDamage(damage, dir, pGen);
    isDead = true;
}

bool WoodenKnife::isCollided(Enemy& target)
{
    float pMinX = position.x - hitBox.x / 2.0f;
    float pMaxX = position.x + hitBox.x / 2.0f;
    float pMinY = position.y - hitBox.y / 2.0f;
    float pMaxY = position.y + hitBox.y / 2.0f;
    float pMinZ = position.z - hitBox.z / 2.0f;
    float pMaxZ = position.z + hitBox.z / 2.0f;

    float cMinX = target.position.x - target.size.x / 2.0f;
    float cMaxX = target.position.x + target.size.x / 2.0f;
    float cMinY = target.position.y - target.size.y / 2.0f;
    float cMaxY = target.position.y + target.size.y / 2.0f;
    float cMinZ = target.position.z - target.size.z / 2.0f;
    float cMaxZ = target.position.z + target.size.z / 2.0f;

    return (pMinX <= cMaxX && pMaxX >= cMinX) &&
           (pMinY <= cMaxY && pMaxY >= cMinY) &&
           (pMinZ <= cMaxZ && pMaxZ >= cMinZ);
}

bool WoodenKnife::getIsDead()
{
    return isDead;
}