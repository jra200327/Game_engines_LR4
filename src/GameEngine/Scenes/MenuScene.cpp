#include "MenuScene.h"

#include "../GameEngine.h"

#include "../../Sample/Systems/DefaultCameraSystem.h"

void MenuScene::Init()
{
    RegisterAction(sf::Mouse::Button::Left, "mouse_left");
    RegisterAction(MouseMove::Move, "mouse_move");
    _mouseClickAction = actionMap["mouse_left"];
    _mouseMoveAction = actionMap["mouse_move"];

    sf::Vector2u size = gameEngine.Window().getSize();
    sf::Vector2f center(size.x / 2.f, size.y / 2.f);
    auto font = gameEngine.Assets().GetFont(AssetNames::BaseFont);

    const uint8_t white[3] = {255, 255, 255};
    _title = std::make_shared<Text>(font, "Level Maker", 48, white);

    _title->SetPosition({center.x - 120, 120.f});

    const uint8_t playTextColor[3] = {255, 255, 255};
    const uint8_t playBgColor[3] = {50, 150, 50};
    const sf::Vector2f playSize = {250.f, 80.f};

    _playButton = std::make_shared<Button>(font, "Play", 32, playSize, playTextColor, playBgColor);

    _playButton->SetPosition({center.x - playSize.x / 2.f, 300.f});

    _playButton->SetCallback([this]()
    {
        std::cout << "Play button clicked!\n";

        gameEngine.LoadScene<GameScene>();
    }); 


    const uint8_t exitTextColor[3] = {255, 255, 255};
    const uint8_t exitBgColor[3] = {180, 50, 50};

    _exitButton = std::make_shared<Button>(font, "Exit", 32, playSize, exitTextColor, exitBgColor);

    _exitButton->SetPosition({center.x - playSize.x / 2.f, 420.f});

    _exitButton->SetCallback([this]()
    {
        std::cout << "Exit button clicked!\n";

        gameEngine.Quit();
    });
    entityFactory = std::make_shared<EntityFactory>(world, gameEngine.Assets());

    sf::Vector2f sizeF(static_cast<float>(size.x), static_cast<float>(size.y));
    entityFactory->CreateEntity("DefaultCamera", sizeF);

    systemsManager.AddSystem(std::make_shared<DefaultCameraSystem>(world));

    systemsManager.Initialize();
}

void MenuScene::Update(float delta)
{
    systemsManager.Update();
    _playButton->Update(_mouseClickAction);
    _exitButton->Update(_mouseClickAction);
}

void MenuScene::Render()
{
    auto& window = gameEngine.Window();

    for (const auto ent : _camera)
    {
        auto& cam = _cameraComponents.Get(ent);
        window.setView(cam.View);
    }

    window.setView(window.getDefaultView());
    _title->Draw(window);

    _playButton->Draw(window);
    _exitButton->Draw(window);
}