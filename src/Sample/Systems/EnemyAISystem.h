#ifndef ENEMYAISYSTEM_H
#define ENEMYAISYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/EnemyComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/ObjectComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/JumpComponent.h"
#include "../Components/GravityComponent.h"


#include "../../GameEngine/GameEngine.h"
#include "EntityFactory.h"
#include "../../GameEngine/Grid/Grid.h"

class EnemyAISystem final : public ISystem
{
    ComponentStorage<EnemyComponent>& _enemyComponents;
    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<ObjectComponent>& _objectComponents;
    ComponentStorage<GravityComponent>& _gravityComponents;
    ComponentStorage<ShooterComponent>& _shooterComponents;
    ComponentStorage<MovementComponent>& _movementComponents;
    ComponentStorage<JumpComponent>& _jumpComponents;
    ComponentStorage<GravityComponent>& _graviteComponents;

    Filter _enemies;
    Filter _objects;
    Filter _player;
    Grid& _grid;

    bool IsBlocked(sf::Vector2i cell);

    std::vector<sf::Vector2i> FindPath(sf::Vector2i start, sf::Vector2i goal, bool allowJump);

    void Move(sf::Vector2f& pos, EnemyComponent& enemy, float dt);

    sf::Vector2i FindEdge(sf::Vector2i start, sf::Vector2i dir);

    bool HasLineOfSight(sf::Vector2i a, sf::Vector2i b);

    void Move(sf::Vector2f& pos, MovementComponent& movement, EnemyComponent& enemy, float dt);

public:
    EnemyAISystem(World& world,
                  Grid& grid)
        : ISystem(world),
         _grid(grid),
         _enemyComponents(world.GetStorage<EnemyComponent>()),
         _positionComponents(world.GetStorage<PositionComponent>()),
         _objectComponents(world.GetStorage<ObjectComponent>()),
         _gravityComponents(world.GetStorage<GravityComponent>()),
         _shooterComponents(world.GetStorage<ShooterComponent>()),
         _movementComponents(world.GetStorage<MovementComponent>()),
         _jumpComponents(world.GetStorage<JumpComponent>()),
         _graviteComponents(world.GetStorage<GravityComponent>()),

         _enemies(FilterBuilder(world)
            .With<EnemyComponent>()
            .With<PositionComponent>()
            .Build()),

        _objects(FilterBuilder(world)
            .With<ObjectComponent>()
            .Build()),

        _player(FilterBuilder(world)
            .With<ShooterComponent>()
            .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif