#ifndef PLAYERANIMATIONSYSTEM_H
#define PLAYERANIMATIONSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/GravityComponent.h"

class PlayerAnimationSystem final : public ISystem
{
    ComponentStorage<SpriteComponent>& _spriteComponents;
    ComponentStorage<AnimationComponent>& _animationComponents;
    ComponentStorage<ShooterComponent>& _shooterComponents;
    ComponentStorage<MovementComponent>& _movementComponents;
    ComponentStorage<GravityComponent>& _gravityComponents;

    Filter _player;

public:
    PlayerAnimationSystem(World& world)
        : ISystem(world),
          _spriteComponents(world.GetStorage<SpriteComponent>()),
          _animationComponents(world.GetStorage<AnimationComponent>()),
          _movementComponents(world.GetStorage<MovementComponent>()),
          _shooterComponents(world.GetStorage<ShooterComponent>()),
          _gravityComponents(world.GetStorage<GravityComponent>()),
          _player(FilterBuilder(world)
              .With<SpriteComponent>()
              .With<AnimationComponent>()
              .With<ShooterComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif