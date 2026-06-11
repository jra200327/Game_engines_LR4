#ifndef SCENE_H
#define SCENE_H

#include "../Ecs/Systems/SystemsManager.h"
#include "../Ecs/World/World.h"
#include "Input/InputAction.h"
#include "Input/InputManager.h"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "../Sample/Systems/EntityFactory.h"

class GameEngine;

class Scene {

    std::shared_ptr<InputAction> RegisterAction(const std::string& name);

protected:
    GameEngine& gameEngine;
    std::unordered_map<std::string, std::shared_ptr<InputAction>> actionMap;

    World world;
    SystemsManager systemsManager;
    std::shared_ptr<EntityFactory> entityFactory;

    void RegisterAction(sf::Keyboard::Key key, const std::string& name);
    void RegisterAction(sf::Mouse::Button btn, const std::string& name);
    void RegisterAction(sf::Mouse::Wheel wheel, const std::string& name);
    void RegisterAction(MouseMove mv, const std::string& name);

public:
    virtual ~Scene() = default;

    // ToDo:
    Scene(GameEngine& gameEngine);

    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void Update(float delta) = 0;
};

#endif //SCENE_H