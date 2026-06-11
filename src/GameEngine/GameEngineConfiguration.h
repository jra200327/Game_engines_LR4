#ifndef GAMEENGINECONF_H
#define GAMEENGINECONF_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// Структуры для конфигурации игровых объектов
struct PlayerConfig {
    float bboxWidth = 30.0f;
    float bboxHeight = 30.0f;
    float speedX = 5.0f;
    float jumpSpeed = 15.0f;
    float maxSpeed = 20.0f;
    float gravity = 1.0f;
    std::string baseAnimation;
    std::vector<std::string> animations;
};

struct BulletConfig {
    std::string texture;
    float speed = 10.0f;
    float circleRadius = 5.0f;
};

struct DecorationConfig {
    std::string name;
    std::string baseTexture;
    std::vector<std::string> animations;
};

struct TileConfig {
    std::string baseTexture;
};

struct BrickConfig {
    std::string baseTexture;
    std::string destroyAnimation;
};

struct LevelObject {
    std::string name;
    int x = 0;
    int y = 0;
};

class GameEngineConfiguration {
public:
    // Параметры окна
    unsigned int width = 1280;
    unsigned int height = 768;
    
    // Имена файлов конфигурации
    std::string assetsFile = "assets.txt";
    std::string levelFile = "level.txt";
    std::string configFile = "config.txt";
    
    // Конфигурации объектов
    PlayerConfig player;
    BulletConfig bullet;
    std::vector<DecorationConfig> decorations;
    TileConfig tile;
    BrickConfig brick;
    
    // Объекты уровня
    std::vector<LevelObject> levelObjects;
    
    // Загрузка основной конфигурации
    bool LoadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open config file: " << path << "\n";
            return false;
        }
        
        std::string line;
        size_t lineNumber = 0;
        
        while (std::getline(file, line)) {
            ++lineNumber;
            
            // Пропускаем комментарии и пустые строки
            if (line.empty() || line[0] == '#') continue;
            
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            
            try {
                if (type == "Window") {
                    iss >> width >> height;
                    std::cout << "[Config] Window: " << width << "x" << height << "\n";
                }
                else if (type == "Player") {
                    player.animations.clear();
                    iss >> player.bboxWidth >> player.bboxHeight 
                        >> player.speedX >> player.jumpSpeed 
                        >> player.maxSpeed >> player.gravity 
                        >> player.baseAnimation;
                    
                    std::string anim;
                    while (iss >> anim) {
                        player.animations.push_back(anim);
                    }
                    std::cout << "[Config] Player configured with " 
                             << player.animations.size() << " animations\n";
                }
                else if (type == "Bullet") {
                    iss >> bullet.texture >> bullet.speed >> bullet.circleRadius;
                    std::cout << "[Config] Bullet configured\n";
                }
                else if (type == "Decoration") {
                    DecorationConfig dec;
                    iss >> dec.name >> dec.baseTexture;
                    
                    std::string anim;
                    while (iss >> anim) {
                        dec.animations.push_back(anim);
                    }
                    
                    decorations.push_back(dec);
                    std::cout << "[Config] Decoration '" << dec.name << "' added\n";
                }
                else if (type == "Tile") {
                    iss >> tile.baseTexture;
                    std::cout << "[Config] Tile configured\n";
                }
                else if (type == "Brick") {
                    iss >> brick.baseTexture >> brick.destroyAnimation;
                    std::cout << "[Config] Brick configured\n";
                }
                else {
                    std::cerr << "Warning: Unknown config type '" << type 
                             << "' at line " << lineNumber << "\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing config at line " << lineNumber 
                         << ": " << e.what() << "\n";
            }
        }
        
        std::cout << "[Config] Configuration loaded successfully\n\n";
        return true;
    }
    
    // Загрузка уровня
    bool LoadLevel(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open level file: " << path << "\n";
            return false;
        }
        
        levelObjects.clear();
        
        std::string line;
        size_t lineNumber = 0;
        
        while (std::getline(file, line)) {
            ++lineNumber;
            
            // Пропускаем комментарии и пустые строки
            if (line.empty() || line[0] == '#') continue;
            
            std::istringstream iss(line);
            LevelObject obj;
            
            if (iss >> obj.name >> obj.x >> obj.y) {
                levelObjects.push_back(obj);
            } else {
                std::cerr << "Warning: Invalid level object at line " << lineNumber << "\n";
            }
        }
        
        std::cout << "[Level] Loaded " << levelObjects.size() << " objects\n";
        return true;
    }
    
    // Полная загрузка всей конфигурации игры
    bool LoadAll() {
        std::cout << "=== Loading Game Configuration ===\n";
        
        if (!LoadFromFile(configFile)) {
            std::cerr << "Failed to load game configuration\n";
            return false;
        }
        
        if (!LoadLevel(levelFile)) {
            std::cerr << "Failed to load level\n";
            return false;
        }
        
        std::cout << "=== Configuration loaded successfully ===\n\n";
        return true;
    }
};

#endif // GAMEENGINECONF_H