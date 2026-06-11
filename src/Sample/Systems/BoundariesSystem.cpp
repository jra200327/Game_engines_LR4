#include "BoundariesSystem.h"


void BoundariesSystem::OnInit()
{

}

void BoundariesSystem::OnUpdate()
{
    int windowWidth = _window.getSize().x;
    int windowHeight = _window.getSize().y;

    for (auto player : _player)
    {
        auto& playerPos = _positionComponents.Get(player);

        if (playerPos.X < 0)
        {
            playerPos.X = 0;
            std::cout << "Stopped player" << std::endl;
        }
        else if(playerPos.X > windowWidth)
        {
            playerPos.X = windowWidth;
            std::cout << "Stopped player" << std::endl;
        }
    }

    for (auto bullet: _bullets)
    {
        auto& bulletPos = _positionComponents.Get(bullet);
        
        if (bulletPos.Y < 0)
        {
            world.RemoveEntity(bullet);
            std::cout << "Deleted bullet" << std::endl;
        }
    }

    for (auto asteroid: _asteroids)
    {
        auto& asteroidPos = _positionComponents.Get(asteroid);
        
        if (asteroidPos.Y > windowHeight + 50 || asteroidPos.X < 0 - 50 || asteroidPos.X > windowWidth +50)
        {
            world.RemoveEntity(asteroid);
            std::cout << "Deleted asteroid" << std::endl;
        }
    }
}