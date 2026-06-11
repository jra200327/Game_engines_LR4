#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <SFML/System/Vector2.hpp>
#include <vector>
#include "BoxColliderComponent.h"

enum class CollisionSide
{
    None,
    Top,
    Bottom,
    Left,
    Right
};

struct CollisionInfo
{
    int entity;
    CollisionSide side;
};

struct CollisionComponent
{
    std::vector<CollisionInfo> Collisions;
};

#endif //COLLISIONCOMPONENT_H