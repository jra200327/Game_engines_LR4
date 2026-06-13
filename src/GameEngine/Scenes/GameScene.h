#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../Scene.h"

#include <memory>
#include <fstream>
#include <string>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../../Sample/Components/PositionComponent.h"
#include "../../Sample/Components/MovementComponent.h"
#include "../../Sample/Components/ShooterComponent.h"
#include "../../Sample/Components/CameraComponent.h"
#include "../../Sample/Components/FollowXCameraTag.h"
#include "../../Sample/Components/EnemyComponent.h"

#include "../../Sample/Systems/RenderSystem.h"

#include "../Grid/Grid.h"
#include "../ColliderDebug/ColliderDebug.h"

class GameScene : public Scene
{
    std::unordered_map<std::string, std::shared_ptr<InputAction>> _actions;

    std::shared_ptr<RenderSystem> _render;

    Grid _grid;
    ColliderDebug _colliderDebug;

    Filter _cameras;
    Filter _players;
    Filter _enemies;

    sf::Vector2f starterPos;

    ComponentStorage<CameraComponent>& _cameraComponents;
    ComponentStorage<PositionComponent>& _positionComponents;

    ComponentStorage<EnemyComponent>& _enemyComponents;

public:
    GameScene(GameEngine& engine) : Scene(engine), _grid(64.f), _colliderDebug(world),
        _cameraComponents(world.GetStorage<CameraComponent>()),
        _positionComponents(world.GetStorage<PositionComponent>()),
        _enemyComponents(world.GetStorage<EnemyComponent>()),
        _cameras(
          FilterBuilder(world)
              .With<CameraComponent>()
              .With<FollowXCameraTag>()
              .Build()),

        _players(
          FilterBuilder(world)
              .With<PositionComponent>()
              .With<ShooterComponent>()
              .Build()),
        _enemies(
            FilterBuilder(world)
            .With<EnemyComponent>()
            .Build())
    {}

    void Init() override;
    void Update(float delta) override;
    void Render() override;
    void RenderSFML();

private:
    void LoadLevel();
};

#endif