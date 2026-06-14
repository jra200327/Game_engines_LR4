#include "GameObjectFactory.h"

#include "../Components/TransformComponent.h"
#include "../Components/BBoxComponent.h"
#include "../Components/EditorComponent.h"
#include "../../Sample/Components/SpriteComponent.h"

namespace Editor {

    GameObjectFactory::GameObjectFactory(World& world, const AssetManager& assets)
        : _world(world), _assets(assets)
    {
    }

    int GameObjectFactory::CreateFromTexture(const std::string& textureName, const sf::Vector2f& worldPos)
    {
        const auto& texture = _assets.GetTexture(textureName);
        sf::Vector2u texSize = texture.getSize();

        int entity = _world.CreateEntity();

        // Transform
        auto& transforms = _world.GetStorage<TransformComponent>();
        transforms.Add(entity, TransformComponent{worldPos, {1.f, 1.f}, 0.f});

        // Sprite
        auto& sprites = _world.GetStorage<SpriteComponent>();
        sprites.Add(entity, SpriteComponent(
            sf::Vector2i(texSize.x, texSize.y),
            sf::Vector2i(0, 0),
            texture,
            0.f,
            1.f
        ));

        // BBox (для отладки и коллизий)
        auto& bboxes = _world.GetStorage<BBoxComponent>();
        bboxes.Add(entity, BBoxComponent{sf::Vector2f(texSize.x, texSize.y)});

        // Маркер редактора
        auto& editors = _world.GetStorage<EditorComponent>();
        editors.Add(entity, EditorComponent{textureName});

        return entity;
    }

}