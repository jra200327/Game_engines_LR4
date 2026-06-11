#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <SFML/Graphics.hpp>

#include "../../Ecs/World/World.h"
#include "../../GameEngine/Assets/AssetManager.h"

class EntityFactory  {
    
   World &_world;
   const AssetManager &_assets;

public:
    EntityFactory (World &wrld, const AssetManager& assets)
        : _world(wrld),
        _assets(assets)
    {
    }

    void CreateEntity(std::string name, sf::Vector2f pos);
};

#endif //ENTITYFACTORY_H