#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <gamecore/enemy.hpp>

enum EnemyType
{
    EYE
};

class EntityManager
{
public:
    static std::unique_ptr<Enemy> spawnEnemy(EnemyType type, glm::vec3 pos);
};