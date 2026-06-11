#ifndef COLLISIONRESOLVESYSTEM_H
#define COLLISIONRESOLVESYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/CollisionComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/ObjectComponent.h"
#include "../Components/FinishTag.h"
#include "../../GameEngine/GameEngine.h"
#include "EntityFactory.h"

class CollisionResolveSystem final : public ISystem {
    
    ComponentStorage<CollisionComponent>& _collisionComponents;
    ComponentStorage<ShooterComponent>& _shooterComponents;
    ComponentStorage<AsteroidComponent>& _asteroidComponents;
    ComponentStorage<BulletComponent>& _bulletComponents;
    ComponentStorage<BoxColliderComponent>& _boxColliderComponents;
    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<GravityComponent>& _gravityComponents;
    ComponentStorage<MovementComponent>& _movementComponents;
    ComponentStorage<ObjectComponent>& _objectComponents;
    ComponentStorage<FinishTag>& _finishComponents;

    Filter _collideables;

    EntityFactory &_factory;
    GameEngine &_engine;

public:
    CollisionResolveSystem(World &world, EntityFactory &factory, GameEngine &engine)
        : ISystem(world),
            _factory(factory),
            _engine(engine),
            _collisionComponents(world.GetStorage<CollisionComponent>()),
            _shooterComponents(world.GetStorage<ShooterComponent>()),
            _asteroidComponents(world.GetStorage<AsteroidComponent>()),
            _bulletComponents(world.GetStorage<BulletComponent>()),
            _boxColliderComponents(world.GetStorage<BoxColliderComponent>()),
            _positionComponents(world.GetStorage<PositionComponent>()),
            _gravityComponents(world.GetStorage<GravityComponent>()),
            _movementComponents(world.GetStorage<MovementComponent>()),
            _objectComponents(world.GetStorage<ObjectComponent>()),
            _finishComponents(world.GetStorage<FinishTag>()),
            _collideables(FilterBuilder(world)
                .With<CollisionComponent>()
                .Build())
    {
    }

    void OnInit() override;

    void OnUpdate() override;
};

#endif //COLLISIONRESOLVESYSTEM_H