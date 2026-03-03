#include <core/player.hpp>
#include <core/camera.hpp>
#include <core/shapes.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

Player::Player(glm::vec3 pos, glm::vec3 s, float sp) : position(pos), size(s), speed(sp) 
{
    camera = new Camera(pos, 800.f, 600.f, 60.f, 0.05f);
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
    
    /*if (onGround && glm::length(velocity) > 0.1f)
    {
        float timer = glfwGetTime() * 10.f;
        camera->position.y += sin(timer) * 0.05f;
    } я хз че это не работает */
    
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

void Player::shoot(Cube* target, Line& line)
{
    glm::vec3 rayOrigin = camera->position;
    glm::vec3 rayDir = camera->front;
    float hitDist;

    if (line.checkCollision(rayOrigin, rayDir, target, hitDist)) 
    {
        std::cout << "BANG!" << "\n";
        
        glm::vec3 hitPoint = rayOrigin + (rayDir * hitDist);
        line.updatePoints(rayOrigin, hitPoint);
    } 
    else 
    {
        line.updatePoints(rayOrigin, rayOrigin + (rayDir * 50.0f));
    }
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