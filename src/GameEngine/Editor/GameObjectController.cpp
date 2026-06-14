#include "GameObjectController.h"
#include <SFML/Graphics/Rect.hpp>

namespace Editor {

    GameObjectController::GameObjectController(World& world)
        : _world(world),
          _transformComponents(world.GetStorage<TransformComponent>()),
          _bboxComponents(world.GetStorage<BBoxComponent>()),
          _editorComponents(world.GetStorage<EditorComponent>()),
          _selectableFilter(FilterBuilder(world)
              .With<TransformComponent>()
              .With<BBoxComponent>()
              .With<EditorComponent>()
              .Build())
    {
    }

    bool GameObjectController::TryStartDrag(const sf::Vector2f& worldPos)
    {
        int foundEntity = -1;
        
        for (auto ent : _selectableFilter)
        {
            auto& transform = _transformComponents.Get(ent);
            auto& bbox = _bboxComponents.Get(ent);

            // Позиция TransformComponent — это ЦЕНТР объекта
            // Поэтому границы считаем от центра минус половина размера
            float halfW = (bbox.Size.x * transform.Scale.x) / 2.0f;
            float halfH = (bbox.Size.y * transform.Scale.y) / 2.0f;

            sf::Rect<float> bounds(
                sf::Vector2f(transform.Position.x - halfW, transform.Position.y - halfH),
                sf::Vector2f(bbox.Size.x * transform.Scale.x, bbox.Size.y * transform.Scale.y)
            );

            if (bounds.contains(worldPos))
            {
                foundEntity = ent;
                break;
            }
        }

        if (foundEntity == -1)
            return false;

        _draggedEntity = foundEntity;
        auto& transform = _transformComponents.Get(foundEntity);
        _dragOffset = transform.Position - worldPos;
        
        return true;
    }

    void GameObjectController::DragTo(const sf::Vector2f& worldPos)
    {
        if (_draggedEntity == -1)
            return;

        auto& transform = _transformComponents.Get(_draggedEntity);
        transform.Position = worldPos + _dragOffset;
    }

    void GameObjectController::StopDrag()
    {
        _draggedEntity = -1;
    }

}