#ifndef COLLISIONRESOLVESYSTEM_H
#define COLLISIONRESOLVESYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/CollisionComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/JumpComponent.h"
#include "../Components/ObjectComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/FinishTag.h"
#include "../../GameEngine/GameEngine.h"
#include "EntityFactory.h"

class CollisionResolveSystem final : public ISystem {
    
    ComponentStorage<CollisionComponent>& _collisionComponents;
    ComponentStorage<ShooterComponent>& _shooterComponents;
    ComponentStorage<BulletComponent>& _bulletComponents;
    ComponentStorage<BoxColliderComponent>& _boxColliderComponents;
    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<GravityComponent>& _gravityComponents;
    ComponentStorage<MovementComponent>& _movementComponents;
    ComponentStorage<ObjectComponent>& _objectComponents;
    ComponentStorage<EnemyComponent>& _enemyComponents;
    ComponentStorage<FinishTag>& _finishComponents;
    ComponentStorage<JumpComponent>& _jumpComponents;

    Filter _collideables;

    EntityFactory &_factory;
    GameEngine &_engine;

    sf::Vector2f& starterPos;

public:
    CollisionResolveSystem(World &world, EntityFactory &factory, GameEngine &engine, sf::Vector2f& startPos)
        : ISystem(world),
            _factory(factory),
            _engine(engine),
            _collisionComponents(world.GetStorage<CollisionComponent>()),
            _shooterComponents(world.GetStorage<ShooterComponent>()),
            _bulletComponents(world.GetStorage<BulletComponent>()),
            _boxColliderComponents(world.GetStorage<BoxColliderComponent>()),
            _positionComponents(world.GetStorage<PositionComponent>()),
            _gravityComponents(world.GetStorage<GravityComponent>()),
            _movementComponents(world.GetStorage<MovementComponent>()),
            _objectComponents(world.GetStorage<ObjectComponent>()),
            _enemyComponents(world.GetStorage<EnemyComponent>()),
            _jumpComponents(world.GetStorage<JumpComponent>()),
            _finishComponents(world.GetStorage<FinishTag>()),
            starterPos(startPos),
            _collideables(FilterBuilder(world)
                .With<CollisionComponent>()
                .Build())
    {
    }

    void OnInit() override;

    void OnUpdate() override;
};

#endif //COLLISIONRESOLVESYSTEM_H