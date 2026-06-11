#ifndef CLEANERSYSTEM_H
#define CLEANERSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Components/AnimationComponent.h"
#include "../Components/ExplosionTag.h"

#include "../../GameEngine/Assets/AssetManager.h"

class CleanerSystem final : public ISystem
{
    ComponentStorage<ExplosionTag>& _explosionComponents;
    ComponentStorage<AnimationComponent>& _animationComponents;

    Filter _explosions;

public:
    CleanerSystem(World& world)
        : ISystem(world),
          _explosionComponents(world.GetStorage<ExplosionTag>()),
          _animationComponents(world.GetStorage<AnimationComponent>()),
          _explosions(FilterBuilder(world)
              .With<AnimationComponent>()
              .With<ExplosionTag>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif