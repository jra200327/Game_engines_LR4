#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <unordered_map>
#include <stdexcept>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Animation.h"

// Глобальные константы для имён ассетов
namespace AssetNames {
    // Текстуры
    inline const std::string TexGoombaMove = "TexGoombaMove";
    inline const std::string TexRun = "TexRun";
    inline const std::string TexIdle = "TexIdle";
    inline const std::string TexJump = "TexJump";
    inline const std::string TexShootRun = "TexShootRun";
    inline const std::string TexShootIdle = "TexShootIdle";
    inline const std::string TexShootJump = "TexShootJump";
    inline const std::string TexExplosion = "TexExplosion";
    inline const std::string Tile = "Tile";
    inline const std::string Tile1 = "Tile1";
    inline const std::string Brick_Tile = "Brick_Tile";
    inline const std::string Question_Tile = "Question_Tile";
    inline const std::string Question_Inactive_Tile = "Question_Inactive_Tile";
    inline const std::string Pipe_Up_R = "Pipe_Up_R";
    inline const std::string Pipe_L = "Pipe_L";
    inline const std::string Pipe_R = "Pipe_R";
    inline const std::string Pipe_Up_L = "Pipe_Up_L";
    inline const std::string Finish = "Finish";
    inline const std::string BigHill = "BigHill";
    inline const std::string Bullet = "Bullet";
    inline const std::string Goomba = "Goomba";
    
    // Анимации
    inline const std::string RunAnim = "RunAnim";
    inline const std::string IdleAnim = "IdleAnim";
    inline const std::string JumpAnim = "JumpAnim";
    inline const std::string ShootRunAnim = "ShootRunAnim";
    inline const std::string ShootIdleAnim = "ShootIdleAnim";
    inline const std::string ShootJumpAnim = "ShootJumpAnim";
    inline const std::string ExplosionAnim = "ExplosionAnim";
    inline const std::string GoombaMoveAnim = "GoombaMoveAnim";
    
    // Шрифты
    inline const std::string BaseFont = "futura";
}

class AssetManager {
private:
    std::unordered_map<std::string, sf::Texture> _textures;
    std::unordered_map<std::string, Animation> _animations;
    std::unordered_map<std::string, sf::Font> _fonts;

public:
    AssetManager() = default;
    
    // Загрузка всех ассетов из конфигурационного файла
    void LoadFromFile(const std::string& path);

    // Добавление ассетов
    void AddTexture(const std::string& name, const std::string& path);
    void AddAnimation(const std::string& name, const std::string& textureName, 
                     size_t frameCount, size_t frameDuration);
    void AddFont(const std::string& name, const std::string& path);

    // Получение ассетов
    const sf::Texture& GetTexture(const std::string& name) const;
    const Animation& GetAnimation(const std::string& name) const;
    const sf::Font& GetFont(const std::string& name) const;

    // Проверка наличия ассетов
    bool HasTexture(const std::string& name) const;
    bool HasAnimation(const std::string& name) const;
    bool HasFont(const std::string& name) const;

    // Получение всех ассетов (для отладки)
    const std::unordered_map<std::string, sf::Texture>& GetTextures() const { return _textures; }
    const std::unordered_map<std::string, Animation>& GetAnimations() const { return _animations; }
    const std::unordered_map<std::string, sf::Font>& GetFonts() const { return _fonts; }
};

#endif // ASSETMANAGER_H