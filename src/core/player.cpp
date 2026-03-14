#include <core/player.hpp>
#include <core/camera.hpp>
#include <core/shapes.hpp>
#include <core/enemy.hpp>
#include <core/shader.hpp>
#include <core/model.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

Player::Player(glm::vec3 pos, glm::vec3 s, float sp, float w, float h) : position(pos), size(s), speed(sp) 
{
    camera = new Camera(pos, w, h, 60.f, 0.05f);
    onGround = false;
}

Player::~Player()
{
    delete camera;
}

void Player::update(GLFWwindow* window, float dt, Cube* plane)
{
    input(window, dt);

    if (!onGround)
    {
        velocity.y -= gravity * dt;
    }
    
    position.y += velocity.y * dt;

    if (isCollided(plane))
    {
        float floorLevel = plane->position.y + plane->size.y / 2.0f;
        
        position.y = floorLevel + size.y / 2.0f;
        
        velocity.y = 0.f;
        onGround = true;
    }
    else
    {
        onGround = false;
    }

    camera->position = position;

    if (shootTimer > 0.f)
        shootTimer -= dt;
}

void Player::input(GLFWwindow* window, float dt)
{
    glm::vec3 moveDir = glm::vec3(0.f);

    glm::vec3 forward = glm::normalize(glm::vec3(camera->front.x, 0.f, camera->front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDir += forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDir -= forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDir -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDir += right;
    
    if (glm::length(moveDir) > 0.0f)
    {
        moveDir = glm::normalize(moveDir);
        
        position += moveDir * speed * dt;
    }
    
    if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && onGround) 
    {
        velocity.y = jumpPower;
        
        onGround = false;
    }
}

void Player::shoot(std::vector<Enemy*> targets, Line& line)
{
    glm::vec3 rayOrigin = camera->position;
    glm::vec3 rayDir = camera->front;
    
    glm::vec3 hitPoint = rayOrigin + (rayDir * 50.0f);
    float hitDist;
    bool hitSomething = false;
    
    for (auto target: targets)
    {
        if (line.checkCollision(rayOrigin, rayDir, target->position, target->size, hitDist)) 
        {
            std::cout << "BANG!" << "\n";
            target->takeDamage(20.f);

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

void Player::drawWeapon(Shader* shader, Model* model, glm::mat4& view, glm::mat4& proj)
{
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

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


bool Player::isCollided(Cube* cube)
{
    float pMinX = position.x - size.x / 2.0f;
    float pMaxX = position.x + size.x / 2.0f;
    float pMinY = position.y - size.y / 2.0f;
    float pMaxY = position.y + size.y / 2.0f;
    float pMinZ = position.z - size.z / 2.0f;
    float pMaxZ = position.z + size.z / 2.0f;

    float cMinX = cube->position.x - cube->size.x / 2.0f;
    float cMaxX = cube->position.x + cube->size.x / 2.0f;
    float cMinY = cube->position.y - cube->size.y / 2.0f;
    float cMaxY = cube->position.y + cube->size.y / 2.0f;
    float cMinZ = cube->position.z - cube->size.z / 2.0f;
    float cMaxZ = cube->position.z + cube->size.z / 2.0f;

    return (pMinX <= cMaxX && pMaxX >= cMinX) &&
           (pMinY <= cMaxY && pMaxY >= cMinY) &&
           (pMinZ <= cMaxZ && pMaxZ >= cMinZ);
}

bool Player::canShoot()
{
    return shootTimer <= 0.f;
}