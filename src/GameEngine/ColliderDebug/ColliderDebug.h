#ifndef COLLIDERDEBUG_H
#define COLLIDERDEBUG_H

#include <SFML/Graphics.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../../Sample/Components/PositionComponent.h"
#include "../../Sample/Components/BoxColliderComponent.h"
#include "../../Sample/Components/CollisionComponent.h"

class ColliderDebug{
    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<BoxColliderComponent>& _boxColliderComponents;

    Filter _collideables;

    bool _col = false;


public:
    ColliderDebug(World &world)
        :   _positionComponents(world.GetStorage<PositionComponent>()),
            _boxColliderComponents(world.GetStorage<BoxColliderComponent>()),
            _collideables(FilterBuilder(world)
                .With<CollisionComponent>()
                .With<PositionComponent>()
                .With<BoxColliderComponent>()
                .Build())
    {
    }

    void Draw(sf::RenderTarget& target);

    bool& GetCol();
};

#endif //COLLIDERDEBUG_H