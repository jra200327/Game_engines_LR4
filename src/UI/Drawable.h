#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SFML/Graphics.hpp>

class Drawable
{
public:
    virtual void Draw(sf::RenderTarget& target) = 0;
    virtual void Update() = 0;
    
    virtual ~Drawable() = default;
};

#endif // DRAWABLE_H