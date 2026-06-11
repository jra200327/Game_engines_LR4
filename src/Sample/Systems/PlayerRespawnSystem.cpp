#include "PlayerRespawnSystem.h"

void PlayerRespawnSystem::OnInit()
{}

void PlayerRespawnSystem::OnUpdate()
{
    for (int ent : _player)
    {
        auto& pos = _positionComponents.Get(ent);
        auto& mov = _movementComponents.Get(ent);
        auto& grav = _gravityComponents.Get(ent);

        if (pos.Y > _maxY)
        {
            pos.X = _starterPos.x;
            pos.Y = _starterPos.y;

            mov.Direction.x = 0;
            mov.Direction.y = 0;
            grav.currentVelocityY = 0;
        }
    }
}
