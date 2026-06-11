#include "InputSystem.h"

#include "imgui.h"
#include "imgui-SFML.h"

void InputSystem::OnInit()
{
}

void InputSystem::OnUpdate()
{
    // Process window events
    while (const std::optional event = _window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(_window, *event);
        if (event->is<sf::Event::Closed>())
        {
            _window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::A)
            {
                _isRightPressed = true;
            }
            else if (keyPressed->code == sf::Keyboard::Key::D)
            {
                _isLeftPressed = true;
            }
            else if (keyPressed->code == sf::Keyboard::Key::Space)
            {
                _isShooting = true;
            }
        }
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            if (keyReleased->code == sf::Keyboard::Key::A)
            {
                _isRightPressed = false;
            }
            else if (keyReleased->code == sf::Keyboard::Key::D)
            {
                _isLeftPressed = false;
            }
            else if (keyReleased->code == sf::Keyboard::Key::Space)
            {
                _isShooting = false;
            }
        }
    }
    
    // Create movement event based on held keys
    MoveDirection dir = MoveDirection::None;
    if (_isLeftPressed && !_isRightPressed)
    {
        dir = MoveDirection::Left;
    }
    else if (_isRightPressed && !_isLeftPressed)
    {
        dir = MoveDirection::Right;
    }
    
    if (dir != MoveDirection::None)
    {
        const int eventEnt = world.CreateEntity();
        _moveEventComponents.Add(eventEnt, MoveInputEvent(dir));
    }

    if (_isShooting)
    {
        const int eventEnt = world.CreateEntity();
        _shootEventComponents.Add(eventEnt, ShootEvent());
    }
}