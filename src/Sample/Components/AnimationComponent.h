#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include "../../GameEngine/Assets/Animation.h"
#include "../../GameEngine/Assets/AssetManager.h"
#include <unordered_map>
#include <string>

struct AnimationComponent
{
    std::unordered_map<std::string, Animation> Animations;

    std::string CurrentAnimation = AssetNames::IdleAnim;

    bool Playing = true;

    Animation& Current()
    {
        return Animations.at(CurrentAnimation);
    }
};

#endif // ANIMATIONCOMPONENT_H