#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"

class AnimationSystem final : public ISystem
{
    ComponentStorage<SpriteComponent>& _spriteComponents;
    ComponentStorage<AnimationComponent>& _animationComponents;

    Filter _animated;

public:
    AnimationSystem(World& world)
        : ISystem(world),
          _spriteComponents(world.GetStorage<SpriteComponent>()),
          _animationComponents(world.GetStorage<AnimationComponent>()),
          _animated(FilterBuilder(world)
              .With<SpriteComponent>()
              .With<AnimationComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif