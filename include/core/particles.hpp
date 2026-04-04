#pragma once

#include <glm/glm.hpp>
#include <vector>

class Shader;
class Camera;

struct Particle
{
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
    glm::vec4 color = glm::vec4(0.f);
    float lifeTime = 0.f;
    float size = 1.f;
    bool active = false;
};

class ParticleGenerator
{
private:
    static ParticleGenerator* instance;

public:
    Particle particles[100];

    ParticleGenerator() { instance = this; }

    void init(float size);
    void spawn(glm::vec3 pos, glm::vec3 vel, glm::vec4 col);
    void update(float dt);
    void draw(Shader* shader, Camera* camera);

    static void emit(glm::vec3 pos, glm::vec3 vel, glm::vec4 col);

    void createExplosion(glm::vec3 pos, glm::vec4 col, int count);

    int firstUnusedParticle();

private:
    unsigned int VAO, VBO;
    int lastUsedParticle = 0;
};