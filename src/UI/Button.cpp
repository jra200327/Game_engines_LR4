#include "Button.h"

#include <iostream>

Button::Button(
    const sf::Font& font,
    const sf::String& text,
    int fontSize,
    const sf::Vector2f& size,
    const uint8_t textColor[3],
    const uint8_t backgroundColor[3])
    : _str(text), _font(font)
{
    _text = sf::Text(_font, _str, fontSize);

    _text.setFillColor(
        sf::Color(
            textColor[0],
            textColor[1],
            textColor[2]));

    _background.setSize(size);

    _background.setFillColor(
        sf::Color(
            backgroundColor[0],
            backgroundColor[1],
            backgroundColor[2]));
}

void Button::Draw(sf::RenderTarget& target)
{
    if (!_shouldDraw)
        return;

    target.draw(_background);

    target.draw(_text);
}

void Button::Update()
{
}

void Button::Update(const std::shared_ptr<InputAction>& mouseAction)
{

    if (mouseAction->Type() != ActionType::Start)
    {
        return;
    }

    sf::Vector2f mousePos(
        static_cast<float>(
            mouseAction->Value2().x),

        static_cast<float>(
            mouseAction->Value2().y));

    if (Contains(mousePos))
    {
        if (_onClick)
        {
            _onClick();
        }
    }
}

void Button::SetPosition(sf::Vector2f position)
{
   _background.setPosition(position);

    sf::Vector2f size = _background.getSize();
    sf::FloatRect textBounds = _text.getLocalBounds();
    _text.setPosition({
        position.x + (size.x / 2.f) - (textBounds.position.x + textBounds.size.x / 2.f),
        position.y + (size.y / 2.f) - (textBounds.position.y + textBounds.size.y / 2.f)
    });
}

void Button::SetCallback(
    std::function<void()> callback)
{
    _onClick = callback;
}

bool Button::Contains(sf::Vector2f point) const
{
    return _background
        .getGlobalBounds()
        .contains(point);
}
