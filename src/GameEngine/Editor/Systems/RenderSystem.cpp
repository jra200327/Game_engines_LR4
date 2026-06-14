#include "RenderSystem.h"
#include "../Components/EditorComponent.h"

namespace Editor {

    RenderSystem::RenderSystem(World& world, sf::RenderWindow& window,
                               std::shared_ptr<CameraService> cameraService,
                               const sf::Font* font)
        : ISystem(world),
          _window(window),
          _cameraService(cameraService),
          _font(font),
          _spriteComponents(world.GetStorage<SpriteComponent>()),
          _transformComponents(world.GetStorage<TransformComponent>()),
          _drawables(FilterBuilder(world)
              .With<SpriteComponent>()
              .With<TransformComponent>()
              .With<EditorComponent>()
              .Build())
    {
        if (_font)
        {
            _gridText.emplace(*_font);
            _gridText->setCharacterSize(12);
            _gridText->setFillColor(sf::Color(255, 255, 255, 180));
        }
    }

    void RenderSystem::OnInit()
    {
    }

    void RenderSystem::OnUpdate()
    {
        // Пусто — рендер перенесён в Render()
    }

    void RenderSystem::DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        sf::Vertex line[] = {
            sf::Vertex(p1, sf::Color(255, 255, 255, 60)),
            sf::Vertex(p2, sf::Color(255, 255, 255, 60))
        };
        _window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    void RenderSystem::RenderGrid()
    {
        const sf::View& view = _cameraService->GetView();
        float currentZoom = _cameraService->GetCurrentZoom();

        sf::Vector2f viewSize = view.getSize();
        sf::Vector2f viewCenter = view.getCenter();

        float leftX = viewCenter.x - viewSize.x / 2.0f;
        float rightX = leftX + viewSize.x;
        float nextGridX = leftX - std::fmod(leftX, GRID_SIZE.x);

        float topY = viewCenter.y - viewSize.y / 2.0f;
        float bottomY = topY + viewSize.y;
        float nextGridY = topY - std::fmod(topY, GRID_SIZE.y);

        // Вертикальные линии
        for (float x = nextGridX; x <= rightX; x += GRID_SIZE.x)
        {
            DrawLine({x, topY}, {x, bottomY});
        }

        // Определяем, рисовать ли подписи
        bool shouldDrawLabels = _gridText.has_value() && (currentZoom < 2.0f);

        // Горизонтальные линии и подписи
        for (float y = nextGridY; y <= bottomY; y += GRID_SIZE.y)
        {
            DrawLine({leftX, y}, {rightX, y});

            if (shouldDrawLabels)
            {
                for (float x = nextGridX; x <= rightX; x += GRID_SIZE.x)
                {
                    int xCell = static_cast<int>(x / GRID_SIZE.x);
                    int yCell = static_cast<int>(y / GRID_SIZE.y);
                    
                    _gridText->setString("(" + std::to_string(xCell) + "," + std::to_string(yCell) + ")");
                    _gridText->setPosition({x + 3, y + 2});
                    _window.draw(*_gridText);
                }
            }
        }
    }

    void RenderSystem::Render()
    {
        RenderGrid();

        for (auto ent : _drawables)
        {
            auto& sprite = _spriteComponents.Get(ent);
            auto& transform = _transformComponents.Get(ent);

            sprite.sprite->setPosition(transform.Position);
            sprite.sprite->setScale(transform.Scale);
            sprite.sprite->setRotation(sf::degrees(transform.Rotation));

            _window.draw(*sprite.sprite);
        }
    }

}