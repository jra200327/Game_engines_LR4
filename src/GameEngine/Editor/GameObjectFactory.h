#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include <SFML/Graphics.hpp>

#include "../../Ecs/World/World.h"
#include "../Assets/AssetManager.h"

namespace Editor {

    class GameObjectFactory
    {
        World& _world;
        const AssetManager& _assets;

    public:
        GameObjectFactory(World& world, const AssetManager& assets);

        // Создать объект из текстуры (возвращает ID сущности)
        int CreateFromTexture(const std::string& textureName, const sf::Vector2f& worldPos);
    };

}

#endif // GAMEOBJECTFACTORY_H