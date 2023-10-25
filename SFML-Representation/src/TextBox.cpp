#include "../include/SFMLTypes.hpp"
#include "../include/TextBox.hpp"
#include <stdint.h>
#include <string>

TextBox::TextBox() = default;

TextBox::TextBox(font* font) noexcept: m_font(font){}

TextBox::TextBox(const TextBox& textBox) noexcept{
    this->rectangle = textBox.rectangle;
    this->keyText = textBox.keyText;
    this->valueText = textBox.valueText;
    this->hash = textBox.hash;
    this->m_font = textBox.m_font;
}

TextBox& TextBox::setOutlineThickness(const float thickness){
    this->rectangle.setOutlineThickness(thickness);
    return *this;
}

TextBox& TextBox::setSize(const vec2f& size) noexcept{
    this->rectangle.setSize(size);
    this->height = size.y;
    this->width = size.x;
    return *this;
}

TextBox& TextBox::setPosition(const vec2f& pos) noexcept{
    this->rectangle.setPosition(pos.x, pos.y);
    return *this;
}

TextBox& TextBox::setFillColor(const color& fill_color) noexcept{
    this->rectangle.setFillColor(fill_color);
    return *this;
}

TextBox& TextBox::setOutlineColor(const color& outline_color) noexcept{
    this->rectangle.setOutlineColor(outline_color);
    return *this;
}

void TextBox::renderToScreen(renderWin& window) const noexcept{
    window.draw(this->rectangle);

    // Drawing the Key, Value and Hash
    window.draw(this->keyText);
    window.draw(this->valueText);
    window.draw(this->hash);
}

void TextBox::writeHashTableText(const std::string_view key, const std::string_view val, const size_t hash, 
    const color& key_color, const color& value_color, const color& hash_color) noexcept{

    this->keyText = {key.data(), *m_font, 16};
    this->valueText = {val.data(), *m_font, 16};
    this->hash = {std::to_string(hash), *m_font, 18};

    vec2f centerPosVal = TextBox::getCenterOfRectangle(this->valueText, this->rectangle);
    vec2f centerPosKey = TextBox::getCenterOfRectangle(this->keyText, this->rectangle);

    this->keyText.setPosition(centerPosKey.x, centerPosKey.y - 25);
    this->valueText.setPosition(centerPosVal);
    this->hash.setPosition(centerPosVal.x + 25, centerPosVal.y + 25);
    
    this->keyText.setFillColor(key_color);
    this->valueText.setFillColor(value_color);
    this->hash.setFillColor(hash_color);
}

void TextBox::writeValueText(const std::string_view valText,
                    const color& valColor, uint16_t characterSize){

    this->valueText = {valText.data(), *m_font, 18};
    this->valueText.setCharacterSize(characterSize);

    vec2f centerValKey = TextBox::getCenterOfRectangle(this->valueText, this->rectangle);

    this->valueText.setPosition(centerValKey);
    this->valueText.setFillColor(valColor);
}

void TextBox::generateTextBox(const vec2f& size, const vec2f& pos, const color& fill_color, 
                    const color& outline_color, const float& thickness) noexcept{
    this->setSize(size).setPosition(pos).setFillColor(fill_color).setOutlineColor(outline_color).setOutlineThickness(thickness);
}

// Boolean functions}
[[nodiscard]] bool TextBox::isEmpty() const noexcept{
    return this->height <= 0 && this->width <= 0;
}

//Static Functions
vec2f TextBox::getCenterOfRectangle(const sftext& text, const sfrectangle& rectangle) noexcept{
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::FloatRect rectangleBounds = rectangle.getGlobalBounds();
    return {
        rectangleBounds.left + (rectangleBounds.width - textBounds.width) / 2.0f,
        rectangleBounds.top + (rectangleBounds.height - textBounds.height) / 2.0f
    };
}

// Getters
vec2f TextBox::getPosition() const noexcept{
    return rectangle.getPosition();
}

vec2f TextBox::getSize() const noexcept{
    return { this->width, this->height };
}

sfrectangle TextBox::getRectangle() const noexcept{
    return this->rectangle;
}