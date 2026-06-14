#ifndef EDITORRENDERSYSTEM_H
#define EDITORRENDERSYSTEM_H

#include <SFML/Graphics.hpp>
#include <optional>

#include "../../../Ecs/Systems/ISystem.h"
#include "../../../Ecs/Filter/Filter.h"
#include "../../../Ecs/Filter/FilterBuilder.h"
#include "../../../Sample/Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../CameraService.h"

namespace Editor {

    class RenderSystem final : public ISystem
    {
        static constexpr sf::Vector2f GRID_SIZE = {64.f, 64.f};

        sf::RenderWindow& _window;
        std::shared_ptr<CameraService> _cameraService;

        const sf::Font* _font = nullptr;
        std::optional<sf::Text> _gridText;

        ComponentStorage<SpriteComponent>& _spriteComponents;
        ComponentStorage<TransformComponent>& _transformComponents;

        Filter _drawables;

        void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2);
        void RenderGrid();

    public:
        RenderSystem(World& world, sf::RenderWindow& window, 
                     std::shared_ptr<CameraService> cameraService,
                     const sf::Font* font = nullptr);

        void OnInit() override;
        void OnUpdate() override;
        
        // Основной метод рендера — вызывается из EditorScene::Render()
        void Render();
    };

}

#endif // EDITORRENDERSYSTEM_H