#include <core/particles.hpp>
#include <core/shader.hpp>
#include <core/camera.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int ParticleGenerator::firstUnusedParticle()
{
    for (int i = lastUsedParticle; i < 100; ++i)
    {
        if (!particles[i].active)
        {
            lastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < lastUsedParticle; ++i)
    {
        if (!particles[i].active)
        {
            lastUsedParticle = i;
            return i;
        }
    }

    return 0;
}

void ParticleGenerator::init(float size)
{
    float vertices[] = { 
        -size,  size, 0,
        -size, -size, 0,
         size, -size, 0,
         size, -size, 0,
         size,  size, 0,
        -size,  size, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glBindVertexArray(0);
}

void ParticleGenerator::spawn(glm::vec3 pos, glm::vec3 vel, glm::vec4 col)
{
    int unused = firstUnusedParticle();
    particles[unused].active = true;
    particles[unused].position = pos;
    particles[unused].velocity = vel;
    particles[unused].color = col;
    particles[unused].lifeTime = 1.f;
}

void ParticleGenerator::update(float dt)
{
    for (auto &p: particles)
    {
        if (p.active)
        {
            p.lifeTime -= dt * 1.5f;
            if (p.lifeTime > 0.f)
            {
                p.velocity.y -= 9.8f * dt;
                p.velocity *= 0.98f;
                p.position += p.velocity * dt;
            }
            else
            {
                p.active = false;
            }
        }
    }
}

void ParticleGenerator::draw(Shader* shader, Camera* camera)
{
    shader->use();
    glBindVertexArray(VAO);

    for (auto &p: particles)
    {
        if (p.active)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, p.position);

            model[0][0] = camera->getView()[0][0]; 
            model[0][1] = camera->getView()[1][0]; 
            model[0][2] = camera->getView()[2][0]; 

            model[1][0] = camera->getView()[0][1]; 
            model[1][1] = camera->getView()[1][1]; 
            model[1][2] = camera->getView()[2][1];

            model = glm::scale(model, glm::vec3(p.size * p.lifeTime));

            shader->setMat4("model", model);
            shader->setVec4("color", p.color);
            shader->setFloat("life", p.lifeTime);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    glBindVertexArray(0);
}

void ParticleGenerator::createExplosion(glm::vec3 pos, glm::vec4 col, int count)
{
    for (int i = 0; i < count; ++i)
    {
        float vx = ((rand() % 100) - 50) / 10.f;
        float vy = ((rand() % 100) - 50) / 10.f;
        float vz = ((rand() % 100) - 50) / 10.f;
        glm::vec3 randVel(vx, vy, vz);

        this->spawn(pos, randVel, col);
    }
}