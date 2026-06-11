#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "Drawable.h"

#include "../GameEngine/Input/InputAction.h"

class Button : public Drawable
{
    sf::RectangleShape _background;

    sf::Font _font;

    sf::Text _text = sf::Text(_font);

    sf::String _str;

    bool _shouldDraw = true;

    std::function<void()> _onClick;

public:
    Button() = default;

    Button(
        const sf::Font& font,
        const sf::String& text,
        int fontSize,
        const sf::Vector2f& size,
        const uint8_t textColor[3],
        const uint8_t backgroundColor[3]);

    void Draw(sf::RenderTarget& target) override;

    void Update() override;

    void Update(const std::shared_ptr<InputAction>& mouseAction);

    void SetPosition(sf::Vector2f position);

    void SetCallback(std::function<void()> callback);

    bool Contains(sf::Vector2f point) const;
};

#endif
