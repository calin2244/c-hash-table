#pragma once
#include <stdint.h>
#include "./SFMLTypes.hpp"
#include <functional>
#include <string>
#include <iostream>
using namespace SFMLAliases;

class TextBox{
    float height;
    float width;
    mutable sf::RectangleShape rectangle;
    sftext keyText;
    sftext valueText;
    sftext hash;
    font m_font;
    
public:
    /*
        * Floats are small enough that passing by value might be faster than by reference
    */
    TextBox(const vec2f& size = {0, 0}): height(size.x), width(size.y){}

    TextBox& setOutlineThickness(const float thickness){
        this->rectangle.setOutlineThickness(thickness);
        return *this;
    }

    TextBox(const TextBox& textBox) noexcept{
        this->rectangle = textBox.rectangle;
        this->keyText = textBox.keyText;
        this->valueText = textBox.valueText;
        this->hash = textBox.hash;
        this->m_font = textBox.m_font;
    }

    TextBox& setSize(const vec2f& size = {5, 5}) noexcept{
        this->rectangle.setSize(size);
        this->height = size.y;
        this->width = size.x;
        return *this;
    }

    TextBox& setPosition(const vec2f& pos = {0.0f, 0.0f}) noexcept{
        this->rectangle.setPosition(pos.x, pos.y);
        return *this;
    }

    TextBox& setFillColor(const color& fill_color = color::White) noexcept{
        this->rectangle.setFillColor(fill_color);
        return *this;
    }

    TextBox& setOutlineColor(const color& outline_color = color::White) noexcept{
        this->rectangle.setOutlineColor(outline_color);
        return *this;
    }
    
    void renderToScreen(renderWin& window) const noexcept{
        window.draw(this->rectangle);

        // Drawing the Key, Value and Hash
        window.draw(this->keyText);
        window.draw(this->valueText);
        window.draw(this->hash);
    }

    void writeHashTableText(const std::string_view key, const std::string_view val, 
        const size_t hash, const font* font, 
        const color& key_color = color::White, const color& value_color = color::White, 
        const color& hash_color = color::White) noexcept{

        this->keyText = {key.data(), *font, 16};
        this->valueText = {val.data(), *font, 16};
        this->hash = {std::to_string(hash), *font, 18};

        vec2f centerPosVal = TextBox::getCenterOfRectangle(this->valueText, this->rectangle);
        vec2f centerPosKey = TextBox::getCenterOfRectangle(this->keyText, this->rectangle);

        this->keyText.setPosition(centerPosKey.x, centerPosKey.y - 25);
        this->valueText.setPosition(centerPosVal);
        this->hash.setPosition(centerPosVal.x + 25, centerPosVal.y + 25);
        
        this->keyText.setFillColor(key_color);
        this->valueText.setFillColor(value_color);
        this->hash.setFillColor(hash_color);
    }

    void writeValueText(const std::string_view valText, const font* font,
                        const color& valColor = color::White){

        this->valueText = {valText.data(), *font, 18};

        vec2f centerValKey = TextBox::getCenterOfRectangle(this->valueText, this->rectangle);

        this->valueText.setPosition(centerValKey);
        this->valueText.setFillColor(valColor);
    }

    bool setFont(const char* path_to_string) noexcept{
        if(!this->m_font.loadFromFile(path_to_string)){
            return false;
        }

        return true;
    }

    void generateTextBox(const vec2f& size, const vec2f& pos, const color& fill_color = color::Black, 
                        const color& outline_color = color::White, const float& thickness = 2) noexcept{
        this->setSize(size).setPosition(pos).setFillColor(fill_color).setOutlineColor(outline_color).setOutlineThickness(thickness);
    }

    bool isEmpty() const noexcept{
        return this->height <= 0 && this->width <= 0;
    }

    sf::RectangleShape getRectangle() const noexcept{
        return this->rectangle;
    }

    //Static Functions
    static vec2f getCenterOfRectangle(const sftext& text, const sfrectangle& rectangle) noexcept{
        sf::FloatRect textBounds = text.getLocalBounds();
        sf::FloatRect rectangleBounds = rectangle.getGlobalBounds();
        return {
            rectangleBounds.left + (rectangleBounds.width - textBounds.width) / 2.0f,
            rectangleBounds.top + (rectangleBounds.height - textBounds.height) / 2.0f
        };
    }

    // Getters
    vec2f getPosition() const noexcept{
        return { this->rectangle.getPosition().x, this->rectangle.getPosition().y };
    }

    vec2f getSize() const noexcept{
        return { this->width, this->height };
    }
};