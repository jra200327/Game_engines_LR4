#include "GameScene.h"

#include "../GameEngine.h"
#include <iostream>
#include <imgui.h>

#include "../../Sample/Systems/RenderSystem.h"
#include "../../Sample/Systems/FollowXCameraSystem.h"
#include "../../Sample/Systems/MovementSystem.h"
#include "../../Sample/Systems/CollisionSystem.h"
#include "../../Sample/Systems/CollisionResolveSystem.h"
#include "../../Sample/Systems/AnimationSystem.h"
#include "../../Sample/Systems/PlayerAnimationSystem.h"
#include "../../Sample/Systems/ShootingSystem.h"
#include "../../Sample/Systems/PlayerRespawnSystem.h"
#include "../../Sample/Systems/CleanerSystem.h"
#include "../../Sample/Systems/EnemyAISystem.h"

void GameScene::Init()
{
    RegisterAction(sf::Keyboard::Key::A, "move_left");
    RegisterAction(sf::Keyboard::Key::D, "move_right");
    RegisterAction(sf::Keyboard::Key::W, "jump");
    RegisterAction(sf::Keyboard::Key::Space, "shoot");
    RegisterAction(sf::Keyboard::Key::Escape, "exit");

    _actions["move_left"] = actionMap["move_left"];
    _actions["move_right"] = actionMap["move_right"];
    _actions["jump"] = actionMap["jump"];
    _actions["shoot"] = actionMap["shoot"];
    _actions["exit"] = actionMap["exit"];

    entityFactory = std::make_shared<EntityFactory>(world, gameEngine.Assets());

    sf::Vector2u size = gameEngine.Window().getSize();
    sf::Vector2f sizeF(static_cast<float>(size.x), static_cast<float>(size.y));
    entityFactory->CreateEntity("FollowXCamera", sizeF);
    _render = std::make_shared<RenderSystem>(world, gameEngine.Window());
    systemsManager.AddSystem(std::make_shared<FollowXCameraSystem>(world));
    systemsManager.AddSystem(std::make_shared<PlayerAnimationSystem>(world));
    systemsManager.AddSystem(std::make_shared<AnimationSystem>(world));
    systemsManager.AddSystem(std::make_shared<MovementSystem>(world, _actions));
    systemsManager.AddSystem(std::make_shared<EnemyAISystem>(world, _grid)); 
    systemsManager.AddSystem(std::make_shared<ShootingSystem>(world, *entityFactory, _actions["shoot"]));
    systemsManager.AddSystem(std::make_shared<CollisionSystem>(world));
    systemsManager.AddSystem(std::make_shared<CollisionResolveSystem>(world, *entityFactory, gameEngine));
    systemsManager.AddSystem(std::make_shared<CleanerSystem>(world));
    
    
    _render->OnInit();
    systemsManager.Initialize();

    LoadLevel();
}

void GameScene::Update(float delta)
{
    if(_actions["exit"]->Type() == ActionType::Start)
    {
        gameEngine.LoadScene<MenuScene>();
    }
    systemsManager.Update();
}

void GameScene::Render()
{
    auto& window = gameEngine.Window();

    window.clear(sf::Color(0x64, 0x64, 0xff));

    for (auto e : _cameras)
    {
        auto& cam = _cameraComponents.Get(e);
        window.setView(cam.View);
        break;
    }

    _render->OnUpdate();

    // TODO: draw world entities (sprites, tiles etc.)
    _grid.Draw(window);
    _colliderDebug.Draw(window);

    window.setView(window.getDefaultView());
    RenderSFML();
}

void GameScene::LoadLevel()
{
    gameEngine._config.LoadLevel("../../Configs/level.txt");
    gameEngine._config.LoadPatrols("../../Configs/enemyPatrol.txt");

    std::vector<int> enemyEntities;
    const auto& levelObjects = gameEngine.Level();

    for (const auto& obj : levelObjects)
    {
        sf::Vector2f worldPos = _grid.GridToWorld(sf::Vector2i(obj.x, obj.y));
        int entity = entityFactory->CreateEntity(obj.name, worldPos);
        if(obj.name == "Player")
        {
            starterPos = _grid.GridToWorld({obj.x, obj.y});
            std::cout<<starterPos.y<<std::endl;
        }
        if(obj.name == "Goomba")
        {
            enemyEntities.push_back(entity);
        }
    }

    std::cout<< "[GameScene] Loaded "<< levelObjects.size()<< " level objects\n";

    sf::Vector2u size = gameEngine.Window().getSize();
    sf::Vector2f sizeF(static_cast<float>(size.x), static_cast<float>(size.y));
    systemsManager.AddSystem(std::make_shared<PlayerRespawnSystem>(world, starterPos, sizeF.y));

     const auto& patrols = gameEngine.Patrols();

    if (patrols.size() < enemyEntities.size())
    {
        std::cerr << "[ERROR] Not enough patrols for enemies!\n";
    }

    int count = std::min(enemyEntities.size(), patrols.size());

    for (int i = 0; i < count; i++)
    {
        auto& enemy = _enemyComponents.Get(enemyEntities[i]);

        enemy.patrolA = patrols[i].a;
        enemy.patrolB = patrols[i].b;

        enemy.currentPatrolTarget = enemy.patrolB;
        enemy.isInitiated = true;
    }

    
}

void GameScene::RenderSFML()
{
    ImGui::Begin("Config settings");
    ImGui::Text("Debug");
    ImGui::Checkbox("Grid", &_grid.GetDrawing());
    ImGui::Checkbox("Colliders", &_colliderDebug.GetCol());
    ImGui::Text("Entity: #| x: ____ | y: _____");
    for (auto entity : _positionComponents.Entities())
    {
        const auto& pos = _positionComponents.Get(entity);
        sf::Vector2i gridPos = _grid.WorldToGrid({pos.X, pos.Y});
        ImGui::Text("Entity %d | x: %d | y: %d", entity, gridPos.x, gridPos.y);
    }

    ImGui::End();
}