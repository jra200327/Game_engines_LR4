#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>

class Grid
{
    float _cellSize;
    sf::Color _gridColor;
    bool _isDrawing = false;

public:
    Grid(float cellSize = 64.f);

    void Draw(sf::RenderTarget& target) const;

    sf::Vector2i WorldToGrid(sf::Vector2f worldPos) const;

    sf::Vector2f GridToWorld(sf::Vector2i gridPos) const;

    float CellSize() const;

    bool& GetDrawing();
};

#endif