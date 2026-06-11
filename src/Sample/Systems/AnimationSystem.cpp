#include "AnimationSystem.h"

void AnimationSystem::OnInit()
{
}

void AnimationSystem::OnUpdate()
{
    for (auto ent : _animated)
    {
        auto& sprite = _spriteComponents.Get(ent);
        auto& animation = _animationComponents.Get(ent);

        if (!animation.Playing)
            continue;

        auto& current = animation.Current();

        current.Update();
        current.ApplyToSprite(*sprite.sprite);
    }
}