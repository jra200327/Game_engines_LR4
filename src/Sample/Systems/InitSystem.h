#ifndef INITSYSTEM_H
#define INITSYSTEM_H

#include "../../Ecs/Systems/IInitializer.h"
#include <SFML/Graphics/Texture.hpp>

#include "EntityFactory.h"

class InitSystem final : public IInitializer {
    sf::Texture &_texture;
    EntityFactory &_factory;

public:
    InitSystem(World &world, sf::Texture &texture, EntityFactory &factory) : IInitializer(world), _texture(texture), _factory(factory) { }

    void OnInit() override;
};



#endif //INITSYSTEM_H
