#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Shader;
struct GLFWwindow;
class Cube;
class Entity;

struct UpdateContext
{
    GLFWwindow* window;
    float dt;
    std::vector<std::unique_ptr<Cube>>& world;

    Entity& player;
    const std::vector<std::unique_ptr<Entity>>& enemies;
};

enum EntityType
{
    PLAYER,
    PROJECTILE,
    ENEMY
};

class Entity
{
public:
    EntityType type;
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
    glm::vec3 size = glm::vec3(0.f);

    float angle = 0.f; // возможно плохое название
    bool isDead = false;


    Entity(EntityType t) : type(t) { }
    virtual ~Entity() = default;

    virtual void update(UpdateContext& ctx) = 0;

    virtual void draw(Shader& shader) = 0;
    virtual void drawHitbox(Shader& shader) = 0;

    bool checkCollision(const Entity& other) const 
    { 
        float pMinX = position.x - size.x / 2.0f;
        float pMaxX = position.x + size.x / 2.0f;
        float pMinY = position.y - size.y / 2.0f;
        float pMaxY = position.y + size.y / 2.0f;
        float pMinZ = position.z - size.z / 2.0f;
        float pMaxZ = position.z + size.z / 2.0f;

        float cMinX = other.position.x - other.size.x / 2.0f;
        float cMaxX = other.position.x + other.size.x / 2.0f;
        float cMinY = other.position.y - other.size.y / 2.0f;
        float cMaxY = other.position.y + other.size.y / 2.0f;
        float cMinZ = other.position.z - other.size.z / 2.0f;
        float cMaxZ = other.position.z + other.size.z / 2.0f;

        return (pMinX <= cMaxX && pMaxX >= cMinX) &&
               (pMinY <= cMaxY && pMaxY >= cMinY) &&
               (pMinZ <= cMaxZ && pMaxZ >= cMinZ);
    }
};