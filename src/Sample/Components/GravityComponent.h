#ifndef GRAVITYCOMPONENT_H
#define GRAVITYCOMPONENT_H

struct GravityComponent {
    float gravity;           
    float currentVelocityY = 0;
    float maxVelocityY = 100;
    bool grounded = false;
     
    
    GravityComponent(float g = 1.0f) : gravity(g) 
    {}
};

#endif