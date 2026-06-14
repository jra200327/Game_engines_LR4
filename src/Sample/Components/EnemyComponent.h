#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H

#include <SFML/System/Vector2.hpp>

enum class EnemyState
{
    Patrol,
    Chase
};

struct EnemyComponent
{
    EnemyState state = EnemyState::Patrol;

    sf::Vector2i patrolA;
    sf::Vector2i patrolB;

    sf::Vector2i currentPatrolTarget;

    std::vector<sf::Vector2i> path;

    float repathTimer = 0.f;

    float sightRadius = 256;
    bool seesPlayer = false;

    bool isInitiated = false;
};

#endif //ENEMYCOMPONENT_H