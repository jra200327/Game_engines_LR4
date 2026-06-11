#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>

#include "InputAction.h"

enum MouseMove { Move };

class GameEngine;

class InputManager
{
    sf::RenderWindow& _window;
    GameEngine& _gameEngine;

    // Для отслеживания состояния клавиш
    std::unordered_map<sf::Keyboard::Key, bool> _keyStates;
    std::unordered_map<sf::Mouse::Button, bool> _mouseBtnStates;

    std::unordered_map<size_t, std::unordered_map<sf::Keyboard::Key, std::shared_ptr<InputAction>>> _actionKeyMaps;
    std::unordered_map<size_t, std::unordered_map<sf::Mouse::Button, std::shared_ptr<InputAction>>> _actionMouseBtnMaps;
    std::unordered_map<size_t, std::unordered_map<sf::Mouse::Wheel, std::shared_ptr<InputAction>>> _actionMouseWheelMaps;
    std::unordered_map<size_t, std::unordered_map<MouseMove, std::shared_ptr<InputAction>>> _actionMouseMoveMaps;

    // Добавляем поддержку удержания клавиш
    std::unordered_map<size_t, std::unordered_map<sf::Keyboard::Key, std::shared_ptr<InputAction>>> _actionKeyHoldMaps;

public:
    InputManager(sf::RenderWindow& window, GameEngine& gameEngine);

    void RegisterInput(size_t scene, sf::Keyboard::Key key, std::shared_ptr<InputAction> action);
    void RegisterInput(size_t scene, sf::Mouse::Button btn, std::shared_ptr<InputAction> action);
    void RegisterInput(size_t scene, sf::Mouse::Wheel wheel, std::shared_ptr<InputAction> action);
    void RegisterInput(size_t scene, MouseMove mv, std::shared_ptr<InputAction> action);
    void RegisterHoldInput(size_t scene, sf::Keyboard::Key key, std::shared_ptr<InputAction> action);

    bool ProcessInput(size_t scene);

    // Методы для проверки состояния клавиш
    bool IsKeyPressed(sf::Keyboard::Key key) const;
    bool IsMouseButtonPressed(sf::Mouse::Button button) const;
};

#endif //INPUTMANAGER_H