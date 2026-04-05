#pragma once
#include <glm/glm.hpp>
#include <core/entity.hpp>
#include <vector>
#include <memory>

class Cube;
class GLFWwindow;
class Camera;
class Line;
class Enemy;
class Shader;
class Model;
class ParticleGenerator;
class Projectile;

class Player : public Entity
{
public:
    Camera* camera = nullptr;
    glm::vec3 dashDir = glm::vec3(0.f);

    float speed = 0.f;
    float health = 100.f;
    float visualHealth = health;
    float dashCooldownTimer = 0.0f;
    float dashCooldown = 1.0f;   
    
    bool onGround = false;
    bool isDashing = false;
    
    Player(glm::vec3 pos, glm::vec3 s, float sp, float w, float h);
    ~Player();
    
    void update(UpdateContext& ctx) override 
        { pUpdate(ctx.window, ctx.dt, ctx.world); }

    void pUpdate(GLFWwindow* window, float dt, std::vector<std::unique_ptr<Cube>>& world);
    void input(GLFWwindow* window, float dt);
    void shoot(std::vector<std::unique_ptr<Entity>>& targets, Line& line);
    void resetShootTimer();
    void takeDamage(float damage);

    void draw(Shader& shader) override { }
    void drawHitbox(Shader& shader) override { }
    void drawWeapon(Shader* shader, Model* model);
    
    bool isCollided(Cube& block);
    bool canShoot();
    
private:
    float gravity = 15.f;
    float jumpPower = 10.f;
    float maxFallSpeed = 125.f;
    
    float cayotTime = 0.15f; // чуть меньше кадра на реакцию (при 60 фпс), еб
    float timeInFall = 0.f;
    
    float dashForce = 20.0f;     
    float dashDuration = 0.1f;  
    float dashTimer = 0.0f;      

    // покачивание камеры при ходьбе
    float bobTimer = 0.0f;
    float bobSpeed = 10.0f;
    float bobAmount = 0.05f;

    float fireRate = 0.5f;
    float shootTimer = 0.f;

    float altFireRate = 0.5f;
    float altShootTimer = 0.f;

    void updateCollide(Cube& block, bool& stateOnGround); // нормальное имя?
};