#include "MouseInputSystem.h"
#include <iostream>

namespace Editor {

    MouseInputSystem::MouseInputSystem(World& world,
                                       std::shared_ptr<CameraService> cameraService,
                                       std::shared_ptr<GameObjectController> goController,
                                       std::unordered_map<std::string, std::shared_ptr<InputAction>>& actionMap)
        : ISystem(world),
          _cameraService(cameraService),
          _goController(goController)
    {
        // ЛЕВАЯ кнопка — перетаскивание камеры
        auto leftIt = actionMap.find("editor_mouse_left");
        if (leftIt != actionMap.end())
            _mouseLeftAction = leftIt->second;
        else
            _mouseLeftAction = std::make_shared<InputAction>("editor_mouse_left");

        // ПРАВАЯ кнопка — перетаскивание объектов
        auto rightIt = actionMap.find("editor_mouse_right");
        if (rightIt != actionMap.end())
            _mouseRightAction = rightIt->second;
        else
            _mouseRightAction = std::make_shared<InputAction>("editor_mouse_right");

        auto moveIt = actionMap.find("editor_mouse_move");
        if (moveIt != actionMap.end())
            _mouseMoveAction = moveIt->second;
        else
            _mouseMoveAction = std::make_shared<InputAction>("editor_mouse_move");

        auto scrollIt = actionMap.find("editor_mouse_scroll");
        if (scrollIt != actionMap.end())
            _mouseScrollAction = scrollIt->second;
        else
            _mouseScrollAction = std::make_shared<InputAction>("editor_mouse_scroll");
    }

    void MouseInputSystem::OnInit()
    {
    }

    void MouseInputSystem::OnUpdate()
    {
        // === ОБРАБОТКА ЛЕВОЙ КНОПКИ МЫШИ (DRAG КАМЕРЫ) ===
        if (_mouseLeftAction->Type() == ActionType::Start)
        {
            sf::Vector2i mousePos = _mouseLeftAction->Value2();
            _cameraService->StartMoving(mousePos);
            _isLeftPressed = true;
        }
        else if (_mouseLeftAction->Type() == ActionType::End)
        {
            _cameraService->StopMoving();
            _isLeftPressed = false;
        }

        // === ОБРАБОТКА ПЕРЕМЕЩЕНИЯ МЫШИ ===
        if (_isLeftPressed)
        {
            // Перетаскивание камеры
            sf::Vector2i mousePos = _mouseMoveAction->Value2();
            _cameraService->Drag(mousePos, 1.0f / 60.0f);
        }
        else if (_isRightPressed)
        {
            // Перетаскивание объекта
            sf::Vector2i mousePos = _mouseMoveAction->Value2();
            sf::Vector2f worldPos = _cameraService->WindowToWorldPosition(mousePos);
            _goController->DragTo(worldPos);
        }

        // === ОБРАБОТКА ПРАВОЙ КНОПКИ МЫШИ (DRAG ОБЪЕКТОВ) ===
        if (_mouseRightAction->Type() == ActionType::Start)
        {
            sf::Vector2i mousePos = _mouseRightAction->Value2();
            sf::Vector2f worldPos = _cameraService->WindowToWorldPosition(mousePos);
            
            // Пробуем начать перетаскивание объекта
            if (_goController->TryStartDrag(worldPos))
            {
                _isRightPressed = true;
                std::cout << "[Editor] Started dragging object\n";
            }
        }
        else if (_mouseRightAction->Type() == ActionType::End)
        {
            if (_isRightPressed)
            {
                _goController->StopDrag();
                _isRightPressed = false;
                std::cout << "[Editor] Stopped dragging object\n";
            }
        }

        // === ОБРАБОТКА СКРОЛЛА (ЗУМ) ===
        if (_mouseScrollAction->Type() == ActionType::Start)
        {
            float scrollDelta = static_cast<float>(_mouseScrollAction->Value());
            sf::Vector2i mousePos = _mouseScrollAction->Value2();
            _cameraService->Zoom(scrollDelta, mousePos);
            _mouseScrollAction->Type() = ActionType::None;
        }
    }

}