#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SFML/Graphics.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/PositionComponent.h"
#include "../Components/SpriteComponent.h"

class RenderSystem final : public ISystem {
    sf::RenderWindow& _window;

    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<SpriteComponent>& _spriteComponents;

    Filter _drawables;

public:
    RenderSystem(World &world, sf::RenderWindow &window)
        : ISystem(world),
            _window(window),
            _positionComponents(world.GetStorage<PositionComponent>()),
            _spriteComponents(world.GetStorage<SpriteComponent>()),
            _drawables(FilterBuilder(world)
                .With<SpriteComponent>()
                .With<PositionComponent>()
                .Build())
    {
    }

    void OnInit() override;

    void OnUpdate() override;
};

#endif //RENDERSYSTEM_H