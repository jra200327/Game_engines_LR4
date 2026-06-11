#include "ColliderDebug.h"

void ColliderDebug::Draw(sf::RenderTarget& target)
{
    if (_col)
{
    for (const auto e : _collideables)
    {
        auto& bbox = _boxColliderComponents.Get(e);
        auto& transform = _positionComponents.Get(e);
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(bbox.Width - 1, bbox.Height - 1));
        rect.setOrigin(sf::Vector2f(bbox.Width/2, bbox.Height/2));
        rect.setPosition({transform.X, transform.Y});
        rect.setFillColor(sf::Color(0, 0, 0, 0));
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(1);
        target.draw(rect);
    }
}
}

bool& ColliderDebug::GetCol()
{
    return _col;
}