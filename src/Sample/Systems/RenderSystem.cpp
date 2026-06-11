#include "RenderSystem.h"

void RenderSystem::OnInit()
{

}

void RenderSystem::OnUpdate()
{
    for (auto ent : _drawables)
    {
        auto& position = _positionComponents.Get(ent);
        auto& sprite = _spriteComponents.Get(ent);
        sprite.sprite->setPosition(sf::Vector2f(position.X, position.Y));
        sf::Vector2f scale = {sprite.scale, sprite.scale};

        if (sprite.flipped)
            scale.x *= -1.f;

            
        sprite.sprite->setScale(scale);
        _window.draw(*sprite.sprite);
    }
}