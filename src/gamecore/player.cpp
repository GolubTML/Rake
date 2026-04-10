#include <gamecore/player.hpp>
#include <gamecore/camera.hpp>
#include <common/shapes.hpp>
#include <gamecore/enemy.hpp>
#include <renderer/shader.hpp>
#include <renderer/model.hpp>
#include <renderer/particles.hpp>
#include <renderer/light.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <gameplay/projectiles/woodenKnife.hpp>

Projectile::SpawnCallback Projectile::onSpawn = nullptr;

Player::Player(glm::vec3 pos, glm::vec3 s, float sp, float w, float h) : Entity(EntityType::PLAYER), speed(sp) 
{
    this->position = pos;
    this->velocity = glm::vec3(0.f);
    this->size     = s;
    this->isDead   = false;

    camera = new Camera(pos, w, h, 90.f, 0.05f);
    lampa = { this->position, 1.f, 0.07f, 0.045f };

    onGround = false;
}

Player::~Player()
{
    delete camera;
}

void Player::pUpdate(GLFWwindow* window, float dt, std::vector<std::unique_ptr<Cube>>& world)
{
    if (dashCooldownTimer > 0.f) dashCooldownTimer -= dt;

    if (isDashing)
    {
        dashTimer -= dt;
        
        position += dashDir * (speed + dashForce) * dt;
        glm::vec3 spawnOrigin = this->position - (camera->front * 0.5f);


        // Тут не правильно работает обработка коллизии когда игрок на полу делает деш. ИСПРАВИТЬ
        if (!onGround)
        {
            bool stateOnGround = false;
            for (auto& block: world)
            {
                if (isCollided(*block))
                {
                    updateCollide(*block, stateOnGround);
                    isDashing = false;
                }
            }
        }

        for (int i = 0; i < 5; ++i)
        {
            glm::vec3 offset = glm::vec3((rand() % 10 - 5) / 20.0f, (rand() % 10 - 5) / 20.0f, (rand() % 10 - 5) / 20.0f);

            glm::vec3 trailVel = glm::vec3(0.0f, 0.1f, 0.0f);

            ParticleGenerator::emit(spawnOrigin + offset, trailVel, glm::vec4(0.4f, 0.7f, 1.0f, 1.0f));
        }

        if (dashTimer <= 0.f)
        {
            isDashing = false;
        }
    }
    else
    {
        float targetTilt = 0.f;
        input(window, dt);

        if (onGround)
            timeInFall = 0.f;
        else
            timeInFall += dt;

        if (!onGround)
        {
            velocity.y -= gravity * dt;

            if (velocity.y < -maxFallSpeed)
                velocity.y = -maxFallSpeed;
        }

        position.y += velocity.y * dt;

        bool currentStateOnGround = false;

        for (auto& block: world)
        {
            if (isCollided(*block))
            {
                updateCollide(*block, currentStateOnGround);
            }
        }

        onGround = currentStateOnGround;

        bool isMoving = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || 
                         glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);

        if (onGround && isMoving)
        {
            bobTimer += dt * bobSpeed;
        }
        else
        {
            bobTimer = glm::mix(bobTimer, 0.0f, dt * 5.0f);
        }

        float bobOffset = sin(bobTimer) * bobAmount;

        camera->position.y += bobOffset;
        
        if (shootTimer > 0.f)
            shootTimer -= dt;

        if (altShootTimer > 0.f)
            altShootTimer -= dt;
    }

    camera->position = position;
    lampa.position = glm::mix(lampa.position, position, dt * 10.f);
    // lampa.position = position;
}

void Player::input(GLFWwindow* window, float dt)
{
    glm::vec3 moveDir = glm::vec3(0.f);

    glm::vec3 forward = glm::normalize(glm::vec3(camera->front.x, 0.f, camera->front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
        moveDir += forward;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    {
        moveDir -= forward;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
    {
        moveDir -= right;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    { 
        moveDir += right;
    }


    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && dashCooldownTimer <= 0.f)
    {
        if (glm::length(moveDir) > 0.f)
        {
            isDashing = true;
            dashDir = glm::normalize(moveDir);
            dashTimer = dashDuration;
            dashCooldownTimer = dashCooldown;

            velocity.y = 0.f;
        }
    }

    if (!isDashing && glm::length(moveDir) > 0.0f)
    {
        moveDir = glm::normalize(moveDir);
        position += moveDir * speed * dt;
    }
    
    if (glm::length(moveDir) > 0.0f)
    {
        moveDir = glm::normalize(moveDir);

        float currentSpeed = speed;

        if (!onGround)
        {
            currentSpeed = speed * 0.5f; // 0.6f кооэфициент
        }
        
        position += moveDir * currentSpeed * dt;
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && (onGround || timeInFall < cayotTime))
    {
        velocity.y = jumpPower;
        
        onGround = false;
        timeInFall = cayotTime;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && altShootTimer <= 0.f)
    {
        glm::vec3 throwDir = camera->front;

        glm::vec3 spawnPos = camera->position + (throwDir * 0.5f) - glm::vec3(0, 0.2f, 0);

        float throwSpeed = 30.0f;
        glm::vec3 initialVelocity = throwDir * throwSpeed;

        Projectile::NewProjectile<WoodenKnife>(spawnPos, initialVelocity, glm::vec3(0.1f), 3.2f, 30.f);

        altShootTimer = altFireRate;
    }
}

void Player::shoot(std::vector<std::unique_ptr<Entity>>& targets, Line& line)
{
    glm::vec3 rayOrigin = camera->position;
    glm::vec3 rayDir = camera->front;
    
    glm::vec3 hitPoint = rayOrigin + (rayDir * 50.0f);
    float hitDist;
    bool hitSomething = false;
    
    for (auto& target: targets)
    {
        Enemy* e = dynamic_cast<Enemy*>(target.get());
        if (line.checkCollision(rayOrigin, rayDir, target->position, target->size, hitDist)) 
        {
            e->takeDamage(20.f, camera->front);

            hitPoint = rayOrigin + (rayDir * hitDist);
            hitSomething = true;

            break;
        } 
    }

    line.updatePoints(rayOrigin, hitPoint);
}

void Player::resetShootTimer()
{
    shootTimer = fireRate;
}

void Player::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0)
    {
        std::cout << "HP below 0!" << "\n";
    }
}

void Player::drawWeapon(Shader* shader, Model* model)
{
    shader->use();
    glm::mat4 gunModel = glm::mat4(1.f);

    gunModel = glm::translate(gunModel, camera->position);

    float yaw = camera->yaw;
    float pitch = camera->pitch;

    gunModel = glm::rotate(gunModel, glm::radians(-camera->yaw - 90.f), glm::vec3(0, 1, 0));
    gunModel = glm::rotate(gunModel, glm::radians(camera->pitch), glm::vec3(1, 0, 0));

    gunModel = glm::translate(gunModel, glm::vec3(0.2f, -0.25f, -0.2f));

    gunModel = glm::rotate(gunModel, glm::radians(0.0f), glm::vec3(0, 1, 0));
    gunModel = glm::scale(gunModel, glm::vec3(0.0525f)); 

    shader->setMat4("model", gunModel);
    model->draw(shader);
}


bool Player::isCollided(Cube& cube)
{
    float pMinX = position.x - size.x / 2.0f;
    float pMaxX = position.x + size.x / 2.0f;
    float pMinY = position.y - size.y / 2.0f;
    float pMaxY = position.y + size.y / 2.0f;
    float pMinZ = position.z - size.z / 2.0f;
    float pMaxZ = position.z + size.z / 2.0f;

    float cMinX = cube.position.x - cube.size.x / 2.0f;
    float cMaxX = cube.position.x + cube.size.x / 2.0f;
    float cMinY = cube.position.y - cube.size.y / 2.0f;
    float cMaxY = cube.position.y + cube.size.y / 2.0f;
    float cMinZ = cube.position.z - cube.size.z / 2.0f;
    float cMaxZ = cube.position.z + cube.size.z / 2.0f;

    return (pMinX <= cMaxX && pMaxX >= cMinX) &&
           (pMinY <= cMaxY && pMaxY >= cMinY) &&
           (pMinZ <= cMaxZ && pMaxZ >= cMinZ);
}

bool Player::canShoot()
{
    return shootTimer <= 0.f;
}

void Player::updateCollide(Cube& block, bool& stateOnGround)
{
    float pMinX = position.x - size.x / 2.0f;
    float pMaxX = position.x + size.x / 2.0f;
    float pMinY = position.y - size.y / 2.0f;
    float pMaxY = position.y + size.y / 2.0f;
    float pMinZ = position.z - size.z / 2.0f;
    float pMaxZ = position.z + size.z / 2.0f;

    float cMinX = block.position.x - block.size.x / 2.0f;
    float cMaxX = block.position.x + block.size.x / 2.0f;
    float cMinY = block.position.y - block.size.y / 2.0f;
    float cMaxY = block.position.y + block.size.y / 2.0f;
    float cMinZ = block.position.z - block.size.z / 2.0f;
    float cMaxZ = block.position.z + block.size.z / 2.0f;

    float overlapX = std::min(pMaxX - cMinX, cMaxX - pMinX);
    float overlapY = std::min(pMaxY - cMinY, cMaxY - pMinY);
    float overlapZ = std::min(pMaxZ - cMinZ, cMaxZ - pMinZ);

    if (overlapY < overlapX && overlapY < overlapZ)
    {
        if (position.y > block.position.y)
        {
            position.y = cMaxY + size.y / 2.0f;
            velocity.y = 0.f;
            stateOnGround = true;
        }
        else
        {
            position.y = cMinY - size.y / 2.0f;
            velocity.y = 0.f;
        }
    }
    else if (overlapX < overlapZ)
    {
        if (position.x > block.position.x) position.x = cMaxX + size.x / 2.0f;
        else position.x = cMinX - size.x / 2.0f;
    }
    else
    {
        if (position.z > block.position.z) position.z = cMaxZ + size.z / 2.0f;
        else position.z = cMinZ - size.z / 2.0f;
    }
}