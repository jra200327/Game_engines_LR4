#include "Text.h"

Text::Text(const Text &other) 
{
    Transfer(other);
}

Text::Text(Text &&other)
{
    Transfer(other);
}

Text::Text(const sf::Font& font, const sf::String text, const int fontSize, const uint8_t fontColor[3]) :
_str(text), _font(font)
{
    _text = sf::Text(_font, _str, fontSize);
    _text.setFillColor(sf::Color(fontColor[0], fontColor[1], fontColor[2]));
}

void Text::Draw(sf::RenderTarget& target)
{
    if(_shoudDraw)
        target.draw(_text);
}
void Text::Update()
{
    
}

void Text::Toggle()
{
    _shoudDraw = !_shoudDraw;
}

const int Text::GetCharacterSize() const
{
    return _text.getCharacterSize();
}

void Text::SetPosition(const sf::Vector2f position)
{
    _text.setPosition(position);
}

Text& Text::operator =(const Text &other)
{
    Transfer(other);

    return *this;
}

Text& Text::operator =(Text &&other)
{
    Transfer(other);

    return *this;
}

void Text::Transfer(const Text& other)
{
    _font = other._font;
    _text = other._text;

    // It is important to note that the sf::Text instance doesn't copy the font that it uses, 
    // it only keeps a reference to it. Thus, a sf::Font must not be destructed while it is 
    // used by a sf::Text (i.e. never write a function that uses a local sf::Font instance 
    // for creating a text). - https://www.sfml-dev.org/documentation/3.0.0/classsf_1_1Text.html
    _text.setFont(_font);
}

void Text::SetText(sf::String str)
{
    _str = str;
    _text.setString(str); 
}