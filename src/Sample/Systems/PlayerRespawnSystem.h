#ifndef PLAYERRESPAWNSYSTEM_H
#define PLAYERRESPAWNSYSTEM_H

#include <SFML/Graphics.hpp>

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Components/ShooterComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/PositionComponent.h"

class PlayerRespawnSystem final : public ISystem
{
    ComponentStorage<MovementComponent>& _movementComponents;
    ComponentStorage<GravityComponent>& _gravityComponents;
    ComponentStorage<PositionComponent>& _positionComponents;

    Filter _player;

    sf::Vector2f _starterPos;
    float _maxY;

public:
    PlayerRespawnSystem(World& world, sf::Vector2f starterPos, float maxY)
        : ISystem(world),
            _starterPos(starterPos),
            _maxY(maxY),
            _movementComponents(world.GetStorage<MovementComponent>()),
            _gravityComponents(world.GetStorage<GravityComponent>()),
            _positionComponents(world.GetStorage<PositionComponent>()),
            _player(FilterBuilder(world)
              .With<ShooterComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //PLAYERRESPAWNSYSTEM_H