#ifndef EDITORGUISYSTEM_H
#define EDITORGUISYSTEM_H

#include <memory>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "../../../Ecs/Systems/ISystem.h"
#include "../../Assets/AssetManager.h"
#include "../GameObjectFactory.h"
#include "../CameraService.h"

namespace Editor {

    class GuiSystem final : public ISystem
    {
        static constexpr sf::Vector2f BTN_SIZE = {64.f, 64.f};

        const AssetManager& _assets;
        std::shared_ptr<GameObjectFactory> _goFactory;
        std::shared_ptr<CameraService> _cameraService;
        sf::RenderWindow& _window;

        // Маппинг: имя текстуры → имя объекта в level.txt
        std::unordered_map<std::string, std::string> _textureToObjectName;

        void SaveScene();

    public:
        GuiSystem(World& world, 
                  sf::RenderWindow& window,
                  const AssetManager& assets,
                  std::shared_ptr<GameObjectFactory> goFactory,
                  std::shared_ptr<CameraService> cameraService);

        void OnInit() override;
        void OnUpdate() override;
    };

}

#endif // EDITORGUISYSTEM_H