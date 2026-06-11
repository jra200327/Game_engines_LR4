#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <SFML/Graphics.hpp>

#include "../../Ecs/Systems/ISystem.h"

#include "../Components/MoveInputEvent.h"
#include "../Components/ShootEvent.h"

class InputSystem : public ISystem {

    sf::RenderWindow& _window; // так делать не надо точно

    ComponentStorage<MoveInputEvent>& _moveEventComponents;
    ComponentStorage<ShootEvent>& _shootEventComponents;

    bool _isLeftPressed = false;
    bool _isRightPressed = false;
    bool _isShooting = false;

public:
    InputSystem(World &world, sf::RenderWindow& window)
        : ISystem(world),
        _window(window),
        _moveEventComponents(world.GetStorage<MoveInputEvent>()),
        _shootEventComponents(world.GetStorage<ShootEvent>())
    {

    }

    void OnInit() override;

    void OnUpdate() override;
};



#endif //INPUTSYSTEM_H
