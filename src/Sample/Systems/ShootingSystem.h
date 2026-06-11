#ifndef SHOOTINGSYSTEM_H
#define SHOOTINGSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../../GameEngine/Input/InputAction.h"

#include "../Components/PositionComponent.h"
#include "../Components/ShooterComponent.h"
#include "EntityFactory.h"

class ShootingSystem final : public ISystem {
    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<ShooterComponent>& _shooterComponents;

    std::shared_ptr<InputAction> _shootAction;

    Filter _shooter;
    
    EntityFactory &_factory;

public:
    ShootingSystem(World &world, EntityFactory &factory, std::shared_ptr<InputAction> action)
        : ISystem(world),
            _shootAction(action),
            _factory(factory),
            _positionComponents(world.GetStorage<PositionComponent>()),
            _shooterComponents(world.GetStorage<ShooterComponent>()),
            _shooter(FilterBuilder(world)
                .With<ShooterComponent>()
                .With<PositionComponent>()
                .Build())
    {
    }

    void OnInit() override;

    void OnUpdate() override;
};

#endif //SHOOTINGSYSTEM_H