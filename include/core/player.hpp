#pragma once
#include <glm/glm.hpp>
#include <vector>

class Cube;
class GLFWwindow;
class Camera;
class Line;
class Enemy;
class Shader;
class Model;
class ParticleGenerator;
class Projectile;

class Player
{
public:
    Camera* camera = nullptr;

    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
    glm::vec3 size = glm::vec3(0.f);
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
    
    void update(GLFWwindow* window, float dt, std::vector<Cube*> world, std::vector<Projectile*>& activeProjetiles, ParticleGenerator& pGen);
    void input(GLFWwindow* window, std::vector<Projectile*>& activeProjetiles, float dt, float& targetTilt);
    void shoot(std::vector<Enemy*> targets, Line& line, ParticleGenerator& pGen);
    void resetShootTimer();
    void takeDamage(float damage);
    void drawWeapon(Shader* shader, Model* model);
    
    bool isCollided(Cube* block);
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

    // поворот камеры при ходьбе
    float currentTilt = 0.0f;
    float maxTilt = 1.2f;       
    float tiltSpeed = 10.0f;

    float fireRate = 0.5f;
    float shootTimer = 0.f;

    float altFireRate = 0.5f;
    float altShootTimer = 0.f;

    void updateCollide(Cube* block, bool& stateOnGround); // нормальное имя?
};