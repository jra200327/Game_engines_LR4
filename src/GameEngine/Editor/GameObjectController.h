#ifndef GAMEOBJECTCONTROLLER_H
#define GAMEOBJECTCONTROLLER_H

#include <SFML/System/Vector2.hpp>

#include "../../Ecs/World/World.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Components/TransformComponent.h"
#include "../Components/BBoxComponent.h"
#include "../Components/EditorComponent.h"

namespace Editor {

    class GameObjectController
    {
        World& _world;

        ComponentStorage<TransformComponent>& _transformComponents;
        ComponentStorage<BBoxComponent>& _bboxComponents;
        ComponentStorage<EditorComponent>& _editorComponents;

        Filter _selectableFilter;

        int _draggedEntity = -1;
        sf::Vector2f _dragOffset;

    public:
        GameObjectController(World& world);

        bool IsDragging() const { return _draggedEntity != -1; }

        // Попытаться начать перетаскивание объекта под мышкой
        // Возвращает true, если объект найден и перетаскивание начато
        bool TryStartDrag(const sf::Vector2f& worldPos);

        // Переместить перетаскиваемый объект
        void DragTo(const sf::Vector2f& worldPos);

        // Закончить перетаскивание
        void StopDrag();
    };

}

#endif // GAMEOBJECTCONTROLLER_H