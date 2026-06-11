#include "AssetManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Вспомогательная функция для обрезки пробелов в начале и конце строки
static std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

void AssetManager::LoadFromFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open assets config file: " + path);
    }

    std::string line;
    size_t lineNumber = 0;
    
    enum class Section {
        None,
        Textures,
        Animations,
        Fonts
    };
    
    Section currentSection = Section::None;
    
    while (std::getline(file, line))
    {
        ++lineNumber;
        
        // Удаляем комментарии (всё после #)
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        
        // Обрезаем пробелы
        line = Trim(line);
        
        // Пропускаем пустые строки
        if (line.empty())
            continue;
        
        // Проверяем секции
        if (line == "Textures:") {
            currentSection = Section::Textures;
            std::cout << "[Assets] Loading textures...\n";
            continue;
        }
        else if (line == "Animations:") {
            currentSection = Section::Animations;
            std::cout << "[Assets] Loading animations...\n";
            continue;
        }
        else if (line == "Fonts:") {
            currentSection = Section::Fonts;
            std::cout << "[Assets] Loading fonts...\n";
            continue;
        }
        
        try
        {
            switch (currentSection) {
                case Section::Textures: {
                    std::istringstream iss(line);
                    std::string name, texturePath;
                    
                    // Читаем все токены из строки
                    std::vector<std::string> tokens;
                    std::string token;
                    while (iss >> token) {
                        tokens.push_back(token);
                    }
                    
                    if (tokens.size() < 2) {
                        std::cerr << "Warning: Invalid texture format at line " << lineNumber << "\n";
                        continue;
                    }
                    
                    // Последний токен - путь к файлу
                    texturePath = tokens.back();
                    tokens.pop_back();
                    
                    // Собираем имя из оставшихся токенов (поддержка пробелов в именах)
                    name = tokens[0];
                    for (size_t i = 1; i < tokens.size(); ++i) {
                        name += "_" + tokens[i];
                    }
                    
                    AddTexture(name, texturePath);
                    std::cout << "  [Texture] " << name << " -> " << texturePath << "\n";
                    break;
                }
                
                case Section::Animations: {
                    std::istringstream iss(line);
                    std::string name, textureName;
                    size_t frameCount, frameDuration;
                    
                    if (!(iss >> name >> textureName >> frameCount >> frameDuration)) {
                        std::cerr << "Warning: Invalid animation format at line " << lineNumber << "\n";
                        continue;
                    }
                    
                    AddAnimation(name, textureName, frameCount, frameDuration);
                    std::cout << "  [Animation] " << name << " (" << frameCount 
                             << " frames, " << frameDuration << "ms per frame)\n";
                    break;
                }
                
                case Section::Fonts: {
                    std::istringstream iss(line);
                    std::string name, fontPath;
                    
                    if (!(iss >> name >> fontPath)) {
                        std::cerr << "Warning: Invalid font format at line " << lineNumber << "\n";
                        continue;
                    }
                    
                    AddFont(name, fontPath);
                    std::cout << "  [Font] " << name << " -> " << fontPath << "\n";
                    break;
                }
                
                default:
                    std::cerr << "Warning: Data outside any section at line " << lineNumber << "\n";
                    break;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error loading asset at line " << lineNumber << ": " << e.what() << "\n";
        }
    }
    
    std::cout << "\n[Assets] Loading completed!\n";
    std::cout << "  Total textures: " << _textures.size() << "\n";
    std::cout << "  Total animations: " << _animations.size() << "\n";
    std::cout << "  Total fonts: " << _fonts.size() << "\n\n";
}

void AssetManager::AddTexture(const std::string &name, const std::string &path)
{
    if (_textures.find(name) != _textures.end())
    {
        std::cerr << "Warning: Texture '" << name << "' already exists, overwriting\n";
    }
    
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        throw std::runtime_error("Failed to load texture: " + path);
    }
    
    _textures[name] = std::move(texture);
}

void AssetManager::AddAnimation(const std::string &name, const std::string &textureName, 
                                const size_t frameCount, const size_t frameDuration)
{
    if (_animations.find(name) != _animations.end())
    {
        std::cerr << "Warning: Animation '" << name << "' already exists, overwriting\n";
    }
    
    auto it = _textures.find(textureName);
    if (it == _textures.end())
    {
        throw std::runtime_error("Texture '" + textureName + "' not found for animation '" + name + "'");
    }
    
    Animation anim;
    anim.SetTexture(it->second);
    anim.SetFrameCount(frameCount);
    anim.SetFrameDuration(frameDuration);
    
    _animations[name] = anim;
}

void AssetManager::AddFont(const std::string &name, const std::string &path)
{
    if (_fonts.find(name) != _fonts.end())
    {
        std::cerr << "Warning: Font '" << name << "' already exists, overwriting\n";
    }
    
    sf::Font font;
    if (!font.openFromFile(path))
    {
        throw std::runtime_error("Failed to load font: " + path);
    }
    
    _fonts[name] = std::move(font);
}

const sf::Texture& AssetManager::GetTexture(const std::string &name) const
{
    auto it = _textures.find(name);
    if (it == _textures.end())
    {
        throw std::runtime_error("Texture not found: " + name);
    }
    return it->second;
}

const Animation& AssetManager::GetAnimation(const std::string &name) const
{
    auto it = _animations.find(name);
    if (it == _animations.end())
    {
        throw std::runtime_error("Animation not found: " + name);
    }
    return it->second;
}

const sf::Font& AssetManager::GetFont(const std::string &name) const
{
    auto it = _fonts.find(name);
    if (it == _fonts.end())
    {
        throw std::runtime_error("Font not found: " + name);
    }
    return it->second;
}

bool AssetManager::HasTexture(const std::string& name) const
{
    return _textures.find(name) != _textures.end();
}

bool AssetManager::HasAnimation(const std::string& name) const
{
    return _animations.find(name) != _animations.end();
}

bool AssetManager::HasFont(const std::string& name) const
{
    return _fonts.find(name) != _fonts.end();
}