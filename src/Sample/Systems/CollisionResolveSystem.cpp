#include "CollisionResolveSystem.h"
#include <vector>


void CollisionResolveSystem::OnInit()
{

}

void CollisionResolveSystem::OnUpdate()
{
    std::vector<int> toDestroy;
    for (auto ent : _collideables)
    {
        bool onGroundThisFrame = false;

        auto& collision = _collisionComponents.Get(ent);

        for (const auto& info : collision.Collisions)
        {
            int other = info.entity;
            CollisionSide side = info.side;

            if (!world.IsEntityAlive(ent) || !world.IsEntityAlive(other))
                continue;
            
            bool isPlayer1 = _shooterComponents.Has(ent);
            bool isBullet1 = _bulletComponents.Has(ent);
            bool isWorld1 = _objectComponents.Has(ent);
            bool isFinish1 = _finishComponents.Has(ent);
            bool isEnemy1 = _enemyComponents.Has(ent);

            bool isPlayer2 = _shooterComponents.Has(other);
            bool isBullet2 = _bulletComponents.Has(other);
            bool isWorld2  = _objectComponents.Has(other);
            bool isFinish2 = _finishComponents.Has(other);
            bool isEnemy2 = _enemyComponents.Has(other);

            if ((isPlayer1 && isBullet2) || (isBullet1 && isPlayer2))
                continue;

            if ((isWorld1 && isBullet2) || (isBullet1 && isWorld2))
            {
                int bullet  = isBullet1 ? ent  : other;
                int world = isWorld1 ? ent : other;

                toDestroy.push_back(bullet);

                auto& object = _objectComponents.Get(world);
                if(object.destroyable)
                {
                    toDestroy.push_back(world);

                    auto& pos = _positionComponents.Get(world);
                    _factory.CreateEntity(AssetNames::ExplosionAnim, {pos.X, pos.Y});
                }

            }

            if ((isEnemy1 && isBullet2) || (isBullet1 && isEnemy2))
            {
                int bullet = isBullet1 ? ent : other;
                int enemy = isEnemy1 ? ent : other;

                toDestroy.push_back(bullet);
                toDestroy.push_back(enemy);

                auto& pos = _positionComponents.Get(enemy);
                _factory.CreateEntity(AssetNames::ExplosionAnim, {pos.X, pos.Y});
            }

            if ((isPlayer1 && isFinish2) || (isFinish1 && isPlayer2))
            {
                int player   = isPlayer1 ? ent   : other;
                int finish = isFinish1 ? ent : other;

                _engine.LoadScene<MenuScene>();

            }

            if (isPlayer1 && isEnemy2)
            {
                int player = ent;
                int enemy = other;

                if (side == CollisionSide::Top)
                {
                    toDestroy.push_back(enemy);
                    auto& pos = _positionComponents.Get(enemy);
                    _factory.CreateEntity(AssetNames::ExplosionAnim, {pos.X, pos.Y});
                }
                else
                {
                    _engine.LoadScene<MenuScene>();
                }
            }

            bool isCharacter1 = isPlayer1 || isEnemy1;

            if (!isCharacter1 || !isWorld2)
                continue;

            auto& pos = _positionComponents.Get(ent);
            auto& box = _boxColliderComponents.Get(ent);

            auto& otherPos = _positionComponents.Get(other);
            auto& otherBox = _boxColliderComponents.Get(other);

            if (side == CollisionSide::Left)
            {
                pos.X = otherPos.X - box.Width;
            }
            else if (side == CollisionSide::Right)
            {
                pos.X = otherPos.X + otherBox.Width;
            }
            else if (side == CollisionSide::Top)
            {
                pos.Y = otherPos.Y - box.Height;
                if (_gravityComponents.Has(ent))
                {
                    auto& grav = _gravityComponents.Get(ent);
                    if(grav.currentVelocityY >= 0)
                    {
                        onGroundThisFrame = true;
                        grav.currentVelocityY = 0;
                    } 
                }
            }
            else if (side == CollisionSide::Bottom)
            {
                pos.Y = otherPos.Y + otherBox.Height;
                if (_gravityComponents.Has(ent))
                {
                    auto& grav = _gravityComponents.Get(ent);
                    grav.currentVelocityY = 0;
                }
                if(isPlayer1 && isWorld2)
                {
                    auto& object = _objectComponents.Get(other);
                    if(object.destroyable)
                {
                    toDestroy.push_back(other);

                    auto& pos = _positionComponents.Get(other);
                    _factory.CreateEntity(AssetNames::ExplosionAnim, {pos.X, pos.Y});
                }
                }
            }
        }
        if(_gravityComponents.Has(ent))
        {
            auto& grav = _gravityComponents.Get(ent);
            grav.grounded = onGroundThisFrame;
        }
    }

    for (auto ent : toDestroy)
    {
        if (world.IsEntityAlive(ent))
        {
            std::cout << "Deleting " << ent << std::endl;
            world.RemoveEntity(ent);
            std::cout << "Deleted " << ent << std::endl;
        }
    }

}