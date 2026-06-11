#ifndef JUMPCOMPONENT_H
#define JUMPCOMPONENT_H

struct JumpComponent
{
    bool jumpRequested = false;
    float jumpSpeed;

    JumpComponent(float speed) : jumpSpeed(speed)
    {}

    JumpComponent()
    {}
};

#endif //JUMPCOMPONENT_H