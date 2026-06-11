#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cstddef>

class Animation {
private:
    sf::Texture _texture;
    size_t _frameCount = 1;
    size_t _frameDuration = 1;
    size_t _currentFrame = 0;
    size_t _frameTimer = 0;
    
public:
    Animation() = default;
    
    void SetTexture(const sf::Texture& texture) { _texture = texture; }
    void SetFrameCount(size_t count) { _frameCount = count; }
    void SetFrameDuration(size_t duration) { _frameDuration = duration; }
    
    const sf::Texture& GetTexture() const { return _texture; }
    size_t GetFrameCount() const { return _frameCount; }
    size_t GetFrameDuration() const { return _frameDuration; }
    size_t GetCurrentFrame() const { return _currentFrame; }
    
    void Update() {
        if (_frameTimer++ >= _frameDuration) {
            _frameTimer = 0;
            _currentFrame = (_currentFrame + 1) % _frameCount;
        }
    }
    
    void Reset() {
        _currentFrame = 0;
        _frameTimer = 0;
    }
    
    void ApplyToSprite(sf::Sprite& sprite) const {
        sprite.setTexture(_texture);
        int frameWidth = _texture.getSize().x / _frameCount;
        sprite.setTextureRect(
    sf::IntRect({static_cast<int>(_currentFrame * frameWidth), 0}, {frameWidth, static_cast<int>(_texture.getSize().y)}));
    }
};

#endif // ANIMATION_H