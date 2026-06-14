#ifndef MOUSEINPUTSYSTEM_H
#define MOUSEINPUTSYSTEM_H

#include <memory>
#include <unordered_map>

#include "../../../Ecs/Systems/ISystem.h"
#include "../../Input/InputAction.h"
#include "../CameraService.h"
#include "../GameObjectController.h"

namespace Editor {

    class MouseInputSystem final : public ISystem
    {
        std::shared_ptr<CameraService> _cameraService;
        std::shared_ptr<GameObjectController> _goController;

        std::shared_ptr<InputAction> _mouseLeftAction;
        std::shared_ptr<InputAction> _mouseRightAction;
        std::shared_ptr<InputAction> _mouseMoveAction;
        std::shared_ptr<InputAction> _mouseScrollAction;

        bool _isLeftPressed = false;
        bool _isRightPressed = false;

    public:
        MouseInputSystem(World& world, 
                         std::shared_ptr<CameraService> cameraService,
                         std::shared_ptr<GameObjectController> goController,
                         std::unordered_map<std::string, std::shared_ptr<InputAction>>& actionMap);

        void OnInit() override;
        void OnUpdate() override;
    };

}

#endif // MOUSEINPUTSYSTEM_H