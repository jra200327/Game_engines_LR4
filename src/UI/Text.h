#ifndef TEXT_H
#define TEXT_H

#include "Drawable.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Drawable.h"

class Text : public Drawable
{
    sf::Font _font; 

    sf::Text _text = sf::Text(_font);

    sf::String _str;

    bool _shoudDraw = true;

    void Transfer(const Text& other);
public:
    Text() = default;
    Text(const Text &other);
    Text(Text &&other);
    Text(const sf::Font& font, const sf::String text, const int fontSize, const uint8_t fontColor[3]);

    const int GetCharacterSize() const;
    void SetPosition(const sf::Vector2f position);

    void Draw(sf::RenderTarget& target) override;
    void Toggle();
    void Update() override;
    void SetText(sf::String str);

    Text& operator =(const Text &other);
    Text& operator =(Text &&other);
};

#endif //TEXT_H