#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SFML/System/Vector2.hpp>

struct TransformComponent
{
    sf::Vector2f Position;
    sf::Vector2f Scale{1.f, 1.f};
    float Rotation = 0.f;
};

#endif // TRANSFORMCOMPONENT_H