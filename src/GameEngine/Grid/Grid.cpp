#include "Grid.h"
#include <cmath>

Grid::Grid(float cellSize)
    : _cellSize(cellSize)
    , _gridColor(80, 80, 80)
{
}

void Grid::Draw(sf::RenderTarget& target) const
{
    if (!_isDrawing)
    {
        return;
    }
    
    sf::View view = target.getView();

    sf::Vector2f center = view.getCenter();
    sf::Vector2f size   = view.getSize();

    float left   = center.x - size.x / 2.f;
    float right  = center.x + size.x / 2.f;
    float top    = center.y - size.y / 2.f;
    float bottom = center.y + size.y / 2.f;

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    float startX = std::floor(left / _cellSize) * _cellSize;
    float startY = std::floor(top / _cellSize) * _cellSize;

    // vertical lines
    for (float x = startX; x <= right; x += _cellSize)
    {
        lines.append(sf::Vertex(sf::Vector2f(x, top), _gridColor));
        lines.append(sf::Vertex(sf::Vector2f(x, bottom), _gridColor));
    }

    // horizontal lines
    for (float y = startY; y <= bottom; y += _cellSize)
    {
        lines.append(sf::Vertex(sf::Vector2f(left, y), _gridColor));
        lines.append(sf::Vertex(sf::Vector2f(right, y), _gridColor));
    }

    target.draw(lines);
}

sf::Vector2i Grid::WorldToGrid(sf::Vector2f worldPos) const
{
    return {
        static_cast<int>(worldPos.x / _cellSize),
        static_cast<int>(worldPos.y / _cellSize)
    };
}

sf::Vector2f Grid::GridToWorld(sf::Vector2i gridPos) const
{
    return {
        gridPos.x * _cellSize + _cellSize * 0.5f,
        gridPos.y * _cellSize + _cellSize * 0.5f
    };
}

float Grid::CellSize() const
{
    return _cellSize;
}

bool& Grid::GetDrawing()
{
    return _isDrawing;
}