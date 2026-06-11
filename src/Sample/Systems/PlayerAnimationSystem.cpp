#include "PlayerAnimationSystem.h"

void PlayerAnimationSystem::OnInit()
{}

void PlayerAnimationSystem::OnUpdate()
{
    for (auto ent : _player)
    {
        auto& anim = _animationComponents.Get(ent);
        auto& mov = _movementComponents.Get(ent);
        auto& shoot = _shooterComponents.Get(ent);
        auto& grav = _gravityComponents.Get(ent);
        if (shoot.shooting)
        {
            if (anim.CurrentAnimation != AssetNames::ShootRunAnim && (mov.Direction.x < 0 || mov.Direction.x > 0) && grav.currentVelocityY >= 0)
            {
                anim.CurrentAnimation = AssetNames::ShootRunAnim;
            }
            else if (anim.CurrentAnimation != AssetNames::ShootIdleAnim && mov.Direction.x == 0 && grav.currentVelocityY >= 0)
            {
                anim.CurrentAnimation = AssetNames::ShootIdleAnim;
            }
            else if (anim.CurrentAnimation != AssetNames::ShootJumpAnim && grav.currentVelocityY < 0)
            {
                anim.CurrentAnimation = AssetNames::ShootJumpAnim;
            }
        }
        else
        {
            if (anim.CurrentAnimation != AssetNames::RunAnim && (mov.Direction.x < 0 || mov.Direction.x > 0) && grav.currentVelocityY >= 0)
            {
                anim.CurrentAnimation = AssetNames::RunAnim;
            }
            else if (anim.CurrentAnimation != AssetNames::IdleAnim && mov.Direction.x == 0 && grav.currentVelocityY >= 0)
            {
                anim.CurrentAnimation = AssetNames::IdleAnim;
            }
            else if (anim.CurrentAnimation != AssetNames::JumpAnim && grav.currentVelocityY < 0)
            {
                anim.CurrentAnimation = AssetNames::JumpAnim;
            }
        }
    }
}
