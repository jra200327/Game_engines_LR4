#include "InputManager.h"

#include <imgui-SFML.h>
#include <iostream>

#include "../GameEngine.h"
#include "imgui.h"

InputManager::InputManager(sf::RenderWindow& window, GameEngine& gameEngine)
    : _window(window)
    , _gameEngine(gameEngine)
{
}

void InputManager::RegisterInput(const size_t scene,
    const sf::Keyboard::Key key, std::shared_ptr<InputAction> action)
{
    _actionKeyMaps[scene][key] = action;
    _keyStates[key] = false; // Инициализируем состояние клавиши
}

void InputManager::RegisterHoldInput(const size_t scene,
    const sf::Keyboard::Key key, std::shared_ptr<InputAction> action)
{
    _actionKeyHoldMaps[scene][key] = action;
    _keyStates[key] = false;
}

void InputManager::RegisterInput(const size_t scene,
    const sf::Mouse::Button btn, std::shared_ptr<InputAction> action)
{
    _actionMouseBtnMaps[scene][btn] = action;
    _mouseBtnStates[btn] = false;
}

void InputManager::RegisterInput(const size_t scene,
    const sf::Mouse::Wheel wheel, std::shared_ptr<InputAction> action)
{
    _actionMouseWheelMaps[scene][wheel] = action;
}

void InputManager::RegisterInput(const size_t scene, const MouseMove mv,
    std::shared_ptr<InputAction> action)
{
    _actionMouseMoveMaps[scene][mv] = action;
}

bool InputManager::ProcessInput(const size_t scene)
{
    // Сброс End экшенов с прошлого кадра в None
    for (auto& [key, action] : _actionKeyMaps[scene])
    {
        if (action->Type() == ActionType::End)
        {
            action->Type() = ActionType::None;
        }
    }
    for (auto& [btn, action] : _actionMouseBtnMaps[scene])
    {
        if (action->Type() == ActionType::Start)
        {
            action->Type() = ActionType::None;
        }
        if (action->Type() == ActionType::End)
        {
            action->Type() = ActionType::None;
        }
    }

    // Сброс Start экшенов для hold действий
    for (auto& [key, action] : _actionKeyHoldMaps[scene])
    {
        if (action->Type() == ActionType::Start)
        {
            action->Type() = ActionType::None;
        }
    }

    // Обработка событий окна
    while (const std::optional event = _window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(_window, *event);
        
        if (event->is<sf::Event::Closed>())
        {
            _window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            _keyStates[keyPressed->code] = true;

            // Обработка обычных нажатий (Start)
            if (_actionKeyMaps[scene].contains(keyPressed->code))
            {
                _actionKeyMaps[scene][keyPressed->code]->Type() = ActionType::Start;
            }
        }
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            _keyStates[keyReleased->code] = false;

            // Обработка обычных отпусканий (End)
            if (_actionKeyMaps[scene].contains(keyReleased->code))
            {
                _actionKeyMaps[scene][keyReleased->code]->Type() = ActionType::End;
            }

            // Обработка отпусканий для hold действий
            if (_actionKeyHoldMaps[scene].contains(keyReleased->code))
            {
                _actionKeyHoldMaps[scene][keyReleased->code]->Type() = ActionType::End;
            }
        }
        else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            _mouseBtnStates[mousePressed->button] = true;

            if (_actionMouseBtnMaps[scene].contains(mousePressed->button))
            {
                _actionMouseBtnMaps[scene][mousePressed->button]->Type() = ActionType::Start;
                _actionMouseBtnMaps[scene][mousePressed->button]->Value2() = mousePressed->position;
            }
        }
        else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            _mouseBtnStates[mouseReleased->button] = false;

            if (_actionMouseBtnMaps[scene].contains(mouseReleased->button))
            {
                _actionMouseBtnMaps[scene][mouseReleased->button]->Type() = ActionType::End;
            }
        }
        else if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if (_actionMouseWheelMaps[scene].contains(mouseWheel->wheel))
            {
                _actionMouseWheelMaps[scene][mouseWheel->wheel]->Value() = static_cast<short>(mouseWheel->delta);
                _actionMouseWheelMaps[scene][mouseWheel->wheel]->Value2() = sf::Mouse::getPosition(_window);
                _actionMouseWheelMaps[scene][mouseWheel->wheel]->Type() = ActionType::Start;
            }
        }
        else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            if (_actionMouseMoveMaps[scene].contains(MouseMove::Move))
            {
                _actionMouseMoveMaps[scene][MouseMove::Move]->Value2() = sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y);
            }
        }
    }

    // Обработка удерживаемых клавиш
    for (auto& [key, action] : _actionKeyHoldMaps[scene])
    {
        if (_keyStates[key])
        {
            if (action->Type() != ActionType::Start)
            {
                action->Type() = ActionType::Start;
            }
        }
    }

    return true;
}

bool InputManager::IsKeyPressed(sf::Keyboard::Key key) const
{
    auto it = _keyStates.find(key);
    return it != _keyStates.end() && it->second;
}

bool InputManager::IsMouseButtonPressed(sf::Mouse::Button button) const
{
    auto it = _mouseBtnStates.find(button);
    return it != _mouseBtnStates.end() && it->second;
}