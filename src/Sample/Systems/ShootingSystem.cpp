#include "ShootingSystem.h"

void ShootingSystem::OnInit()
{

}

void ShootingSystem::OnUpdate()
{
    if (_shootAction->Type() == ActionType::Start)
    {
        for (const auto ent : _shooter)
        {
            auto& position = _positionComponents.Get(ent);
            auto& shoot = _shooterComponents.Get(ent);

            if(shoot.timer <= 0)
            {
                _factory.CreateEntity("Bullet", sf::Vector2f(position.X, position.Y));
                shoot.timer = shoot.cd;
                std::cout << "Pew" << std::endl;
            }
            shoot.shooting = true;
        }
    }
    else
    {
        for (const auto ent : _shooter)
        {
            auto& shoot = _shooterComponents.Get(ent);
            shoot.shooting = false;
        }
    }

    for (const auto ent : _shooter)
    {
        auto& shoot = _shooterComponents.Get(ent);
        shoot.timer--;
    }
}