#include "CleanerSystem.h"

void CleanerSystem::OnInit()
{}

void CleanerSystem::OnUpdate()
{
    for (int ent : _explosions)
    {
        auto& anim = _animationComponents.Get(ent);
        if(anim.Animations[AssetNames::ExplosionAnim].GetCurrentFrame() == anim.Animations[AssetNames::ExplosionAnim].GetFrameCount()-1)
        {
            world.RemoveEntity(ent);
        }
    }
}