#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/PositionComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/CollisionComponent.h"

class CollisionSystem final : public ISystem {
    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<BoxColliderComponent>& _boxColliderComponents;
    ComponentStorage<CircleColliderComponent>& _circleColliderComponents;
    ComponentStorage<CollisionComponent>& _collisionComponents;

    Filter _collideables;

    void Print(int ent1, int ent2);  // Это тоже можно вынести в отдельную систему

public:
    CollisionSystem(World &world)
        : ISystem(world), 
            _positionComponents(world.GetStorage<PositionComponent>()),
            _boxColliderComponents(world.GetStorage<BoxColliderComponent>()),
            _circleColliderComponents(world.GetStorage<CircleColliderComponent>()),
            _collisionComponents(world.GetStorage<CollisionComponent>()),
            _collideables(FilterBuilder(world)
                .With<CollisionComponent>()
                .With<PositionComponent>()
                .Build())
    {
    }

    void OnInit() override;

    void OnUpdate() override;
};

#endif //COLLISIONSYSTEM_H