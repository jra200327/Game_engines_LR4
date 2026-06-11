#include "MovementSystem.h"

void MovementSystem::Print(int ent)
{
    auto& position = _positionComponents.Get(ent);

    std::cout << ent << " Pos: " << position.X << ", " << position.Y << std::endl;
}

void MovementSystem::OnInit()
{

}

void MovementSystem::OnUpdate()
{
        for (const auto ent : _player)
        {
            auto& movement = _movementComponents.Get(ent);
            auto& sprite = _spriteComponents.Get(ent);
            auto& gravity = _gravityComponents.Get(ent);
            auto& jump = _jumpComponents.Get(ent);

            movement.Direction.x = 0.f;

            if (_actions["move_left"]->Type() == ActionType::Start)
            {
                movement.Direction.x -= 1.f;
                sprite.flipped = true;
            }

            if (_actions["move_right"]->Type() == ActionType::Start)
            {
                movement.Direction.x += 1.f;
                sprite.flipped = false;
            }
            if (_actions["jump"]->Type() == ActionType::Start)
            {
                if (gravity.grounded)
                {
                    jump.jumpRequested = true;
                }
            }
        }

        for (const auto ent : _gravityEntities)
        {   
            auto& position = _positionComponents.Get(ent);
            auto& movement = _movementComponents.Get(ent);
            auto& gravity  = _gravityComponents.Get(ent);

            if (_jumpComponents.Has(ent))
            {
                auto& jump = _jumpComponents.Get(ent);
                if (jump.jumpRequested)
                {
                    gravity.currentVelocityY = -jump.jumpSpeed;
                    gravity.grounded = false;
                    jump.jumpRequested = false;
                }
            }

            position.X += movement.Speed * movement.Direction.x;

            if (!gravity.grounded)
            {
                gravity.currentVelocityY += gravity.gravity;
                if(gravity.currentVelocityY > gravity.maxVelocityY)
                    gravity.currentVelocityY = gravity.maxVelocityY;
                if (gravity.currentVelocityY < -gravity.maxVelocityY)
                    gravity.currentVelocityY = -gravity.maxVelocityY;
            }
            else
            {
                gravity.currentVelocityY = 0.f;
            }

            position.Y += gravity.currentVelocityY;
        }

        for (const auto ent : _moveables)
        {
            bool hasGravity = false;
            for (const auto gravEnt : _gravityEntities)
            {
                if (ent == gravEnt)
                {
                    hasGravity = true;
                    break;
                }
            }
        
            if (hasGravity) continue;
        
            auto& position = _positionComponents.Get(ent);
            auto& movement = _movementComponents.Get(ent);
        
            position.X += movement.Speed * movement.Direction.x;
            position.Y += movement.Speed * movement.Direction.y;
        }
}
