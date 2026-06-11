#include "GameEngine.h"
#include "Assets/AssetManager.h"
#include <imgui-SFML.h>
#include <imgui.h>

void GameEngine::Render(float delta)
{
    _window.clear(sf::Color::Black);
    ImGui::SFML::Update(
        _window,
        sf::seconds(delta));

    CurrentScene()->Render();
    ImGui::SFML::Render(_window);
    _window.display();
}

GameEngine::GameEngine(const GameEngineConfiguration& config) : _config(config)
{
     _window.create(sf::VideoMode({static_cast<unsigned int>(_config.width), static_cast<unsigned int>(_config.height)}), "Level maker");
     auto desktop = sf::VideoMode::getDesktopMode();
    _window.setPosition({ (int) (desktop.size.x / 2 - _config.width / 2), (int) (desktop.size.y / 2 - _config.height / 2) });

    _window.setFramerateLimit(60);

    ImGui::SFML::Init(_window);
    _inputManager = std::make_shared<InputManager>(_window, *this);
    Initialize();
}

void GameEngine::Initialize()
{
    _assetManager.LoadFromFile("../../Configs/assets.txt");
    _config.LoadLevel("../../Configs/level.txt");
    LoadScene<MenuScene>();
}

void GameEngine::Run()
{
    std::cout << "Run started\n" << std::endl;
    _isRunning = true;
     while (_isRunning && _window.isOpen())
    {
        const float delta = _deltaClock.restart().asSeconds();

        auto scene = CurrentScene();
        if (!scene) continue;
        _inputManager->ProcessInput(_currentScene);

        scene->Update(delta);

        Render(delta);
    }

    ImGui::SFML::Shutdown();
}

void GameEngine::Quit()
{
    _isRunning = false;

    _window.close();
}