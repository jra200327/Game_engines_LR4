#ifndef BOUNDARIESSYSTEM_H
#define BOUNDARIESSYSTEM_H

#include <SFML/Graphics.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/ShooterComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/PositionComponent.h"

class BoundariesSystem final : public ISystem {

    sf::RenderWindow& _window;

    ComponentStorage<PositionComponent>& _positionComponents;

    Filter _bullets;
    Filter _asteroids;
    Filter _player;

public:
    BoundariesSystem(World &world, sf::RenderWindow &window)
        : ISystem(world),
            _window(window),
            _positionComponents(world.GetStorage<PositionComponent>()),
            _bullets(FilterBuilder(world)
                .With<BulletComponent>()
                .Build()),
            _asteroids(FilterBuilder(world)
                .With<AsteroidComponent>()
                .Build()),
            _player(FilterBuilder(world)
                .With<ShooterComponent>()
                .Build())
    {
    }

    void OnInit() override;

    void OnUpdate() override;
};

#endif //BOUNDARIESSYSTEM_H