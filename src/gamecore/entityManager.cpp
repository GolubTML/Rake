#include <gamecore/entityManager.hpp>
#include <gamecore/enemy.hpp>
#include <gameplay/enemy/eye.hpp>
#include <common/shapes.hpp>

std::unique_ptr<Enemy> EntityManager::spawnEnemy(EnemyType type, glm::vec3 pos)
{
    switch (type)
    {
    case EnemyType::EYE:
        return std::make_unique<Eye>(pos, glm::vec3(0.7f), 100.f);
    
    default:
        return nullptr;
    }
}