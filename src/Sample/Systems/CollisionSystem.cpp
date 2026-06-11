#include "CollisionSystem.h"

#include "CollisionSystem.h"
#include <algorithm>
#include <iostream>

static CollisionSide OppositeSide(CollisionSide side)
{
    switch (side)
    {
        case CollisionSide::Top: return CollisionSide::Bottom;
        case CollisionSide::Bottom: return CollisionSide::Top;
        case CollisionSide::Left: return CollisionSide::Right;
        case CollisionSide::Right: return CollisionSide::Left;
        default: return CollisionSide::None;
    }
}

static CollisionSide GetAABBSide(const sf::Vector2f& posA, const sf::Vector2f& sizeA, const sf::Vector2f& posB, const sf::Vector2f& sizeB)
{
    float leftA = posA.x;
    float rightA = posA.x + sizeA.x;
    float topA = posA.y;
    float bottomA = posA.y + sizeA.y;

    float leftB = posB.x;
    float rightB = posB.x + sizeB.x;
    float topB = posB.y;
    float bottomB = posB.y + sizeB.y;

    float overlapX = std::min(rightA, rightB) - std::max(leftA, leftB);
    float overlapY = std::min(bottomA, bottomB) - std::max(topA, topB);

    if (overlapX <= 0 || overlapY <= 0)
        return CollisionSide::None;

    if (overlapX < overlapY)
    {
        return (posA.x < posB.x) ? CollisionSide::Left : CollisionSide::Right;
    }
    else
    {
        return (posA.y < posB.y) ? CollisionSide::Top : CollisionSide::Bottom;
    }
}


void CollisionSystem::Print(int ent1, int ent2)
{
    std::cout << ent1 << " collided with: " << ent2 << std::endl;
}

void CollisionSystem::OnInit()
{

}

void CollisionSystem::OnUpdate()
{
    for (const auto ent : _collideables)
    {
        auto& collision = _collisionComponents.Get(ent);
        collision.Collisions.clear();
    }

    for (auto it1 = _collideables.begin(); it1 != _collideables.end(); ++it1)
    {
        auto ent1 = *it1;

        auto& position1 = _positionComponents.Get(ent1);
        auto& collision1 = _collisionComponents.Get(ent1);

        bool hasBox1 = _boxColliderComponents.Has(ent1);
        bool hasCircle1 = _circleColliderComponents.Has(ent1);

        auto it2 = it1;
        ++it2;

        for (; it2 != _collideables.end(); ++it2)
        {
            auto ent2 = *it2;

            auto& position2 = _positionComponents.Get(ent2);
            auto& collision2 = _collisionComponents.Get(ent2);

            bool hasBox2 = _boxColliderComponents.Has(ent2);
            bool hasCircle2 = _circleColliderComponents.Has(ent2);

            bool collided = false;
            CollisionSide side = CollisionSide::None;
            if (hasBox1 && hasBox2)
            {
                auto& box1 = _boxColliderComponents.Get(ent1);
                auto& box2 = _boxColliderComponents.Get(ent2);

                float left1 = position1.X;
                float top1 = position1.Y;

                float left2 = position2.X;
                float top2 = position2.Y;

                collided =
                    left1 < left2 + box2.Width &&
                    left2 < left1 + box1.Width &&
                    top1 < top2 + box2.Height &&
                    top2 < top1 + box1.Height;

                if (collided)
                {
                    side = GetAABBSide(
                        {position1.X, position1.Y},
                        {box1.Width, box1.Height},
                        {position2.X, position2.Y},
                        {box2.Width, box2.Height}
                    );
                }
            }
            else if (hasCircle1 && hasCircle2)
            {
                auto& c1 = _circleColliderComponents.Get(ent1);
                auto& c2 = _circleColliderComponents.Get(ent2);

                float dx = position1.X - position2.X;
                float dy = position1.Y - position2.Y;

                float r = c1.Radius + c2.Radius;

                collided = (dx * dx + dy * dy) <= (r * r);

                side = CollisionSide::None;
            }
            else if (hasBox1 && hasCircle2)
            {
                auto& box = _boxColliderComponents.Get(ent1);
                auto& circle = _circleColliderComponents.Get(ent2);

                float boxLeft = position1.X;
                float boxTop = position1.Y;
                float boxRight = boxLeft + box.Width;
                float boxBottom = boxTop + box.Height;

                float closestX = std::max(boxLeft, std::min(position2.X, boxRight));
                float closestY = std::max(boxTop, std::min(position2.Y, boxBottom));

                float dx = position2.X - closestX;
                float dy = position2.Y - closestY;

                collided = (dx * dx + dy * dy) <= (circle.Radius * circle.Radius);

                side = CollisionSide::None;
            }
            else if (hasCircle1 && hasBox2)
            {
                auto& box = _boxColliderComponents.Get(ent2);
                auto& circle = _circleColliderComponents.Get(ent1);

                float boxLeft = position2.X;
                float boxTop = position2.Y;
                float boxRight = boxLeft + box.Width;
                float boxBottom = boxTop + box.Height;

                float closestX = std::max(boxLeft, std::min(position1.X, boxRight));
                float closestY = std::max(boxTop, std::min(position1.Y, boxBottom));

                float dx = position1.X - closestX;
                float dy = position1.Y - closestY;

                collided = (dx * dx + dy * dy) <= (circle.Radius * circle.Radius);

                side = CollisionSide::None;
            }
            if (collided)
            {
                collision1.Collisions.push_back({ent2, side});
                collision2.Collisions.push_back({ent1, OppositeSide(side)});
            }
        }
    }
}