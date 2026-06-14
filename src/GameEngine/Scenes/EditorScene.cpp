#include "EditorScene.h"
#include "../GameEngine.h"
#include "../Editor/CameraService.h"
#include "../Editor/GameObjectFactory.h"
#include "../Editor/GameObjectController.h"
#include "../Editor/Systems/MouseInputSystem.h"
#include "../Editor/Systems/RenderSystem.h"
#include "../Editor/Systems/GuiSystem.h"

#include <iostream>

EditorScene::EditorScene(GameEngine& engine)
    : Scene(engine)
{
}

void EditorScene::Init()
{
    std::cout << "[EditorScene] Init\n";
    
    // === РЕГИСТРИРУЕМ ДЕЙСТВИЯ ВВОДА ===
    RegisterAction(sf::Keyboard::Key::Escape, "exit");
    
    // Мышь
    RegisterAction(sf::Mouse::Button::Left, "editor_mouse_left");
    RegisterAction(sf::Mouse::Button::Right, "editor_mouse_right");
    RegisterAction(MouseMove::Move, "editor_mouse_move");
    RegisterAction(sf::Mouse::Wheel::Vertical, "editor_mouse_scroll");
    
    // === СОЗДАЁМ СЕРВИСЫ ===
    sf::Vector2u windowSize = gameEngine.Window().getSize();
    sf::Vector2f sizeF(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    
    _cameraService = std::make_shared<Editor::CameraService>(gameEngine.Window(), sizeF);
    _goFactory = std::make_shared<Editor::GameObjectFactory>(world, gameEngine.Assets());
    _goController = std::make_shared<Editor::GameObjectController>(world);
    
    // === ДОБАВЛЯЕМ СИСТЕМЫ ===
    // Система ввода мыши (камера + drag объектов)
    systemsManager.AddSystem(std::make_shared<Editor::MouseInputSystem>(
        world, _cameraService, _goController, actionMap));
    
    // Система рендера (сетка + объекты)
    const sf::Font* font = nullptr;
    if (gameEngine.Assets().HasFont(AssetNames::BaseFont))
        font = &gameEngine.Assets().GetFont(AssetNames::BaseFont);
    
    _renderSystem = std::make_shared<Editor::RenderSystem>(
        world, gameEngine.Window(), _cameraService, font);
    
    systemsManager.AddSystem(_renderSystem);
    
    // GUI система (ImGui)
    systemsManager.AddSystem(std::make_shared<Editor::GuiSystem>(
        world, gameEngine.Window(), gameEngine.Assets(), _goFactory, _cameraService));
    
    systemsManager.Initialize();
}

void EditorScene::Update(float delta)
{
    if (actionMap["exit"]->Type() == ActionType::Start)
    {
        gameEngine.LoadScene<MenuScene>();
        return;
    }
    
    systemsManager.Update();
}

void EditorScene::Render()
{
    auto& window = gameEngine.Window();
    window.clear(sf::Color(100, 100, 255));
    
    // Устанавливаем view камеры редактора
    window.setView(_cameraService->GetView());
    
    // Рисуем сетку и объекты через RenderSystem
    _renderSystem->Render();
    
    // Возвращаем дефолтный view для ImGui
    window.setView(window.getDefaultView());
}