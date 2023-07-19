#pragma once
#include <stdint.h>

#include "../SFML/include/SFML/Graphics.hpp"
#include <functional>
#include <string>
#include <iostream>

using vec2f = sf::Vector2<float>;
using vec2i = sf::Vector2<int>;
using color = sf::Color;

class TextBox{
    mutable sf::RectangleShape rectangle;
    sf::Text keyText;
    sf::Text valueText;
    sf::Text hash;
    sf::Font m_font;
    public:
    float height;
    float width;
        TextBox(): height(0), width(0){}
        TextBox(const float& height, const float& width): height(height), width(width){}

        TextBox& setOutlineThickness(const float& thickness){
            this->rectangle.setOutlineThickness(thickness);
            return *this;
        }

        TextBox& setSize(const vec2f& size = {5, 5}){
            this->rectangle.setSize(size);
            this->height = height;
            this->width = width;
            return *this;
        }

        TextBox& setPosition(const vec2f& pos = {0.0f, 0.0f}){
            this->rectangle.setPosition(pos.x, pos.y);
            return *this;
        }

        TextBox& setFillColor(const color& fill_color = color::White){
            this->rectangle.setFillColor(fill_color);
            return *this;
        }

        TextBox& setOutlineColor(const color& outline_color = color::White){
            this->rectangle.setOutlineColor(outline_color);
            return *this;
        }
        
        void renderToScreen(sf::RenderWindow& window){
            window.draw(this->rectangle);

            // Drawing the Key, Value and Hash
            window.draw(this->keyText);
            window.draw(this->valueText);
            window.draw(this->hash);
        }

        void writeText(const std::string_view& key, const std::string_view& val, 
            const size_t& hash, const sf::Font* font, 
            const color& key_color = color::White, const color& value_color = color::White){

            sf::FloatRect bounds = rectangle.getGlobalBounds();
            vec2f bnds = {bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f};

            this->keyText = {key.data(), *font, 18};
            this->valueText = {val.data(), *font, 18};
            this->hash = {std::to_string(hash), *font, 14};

            sf::FloatRect textValueBound = this->valueText.getLocalBounds();
            sf::FloatRect textKeyBound = this->keyText.getLocalBounds();
            sf::FloatRect rectangleBounds = this->rectangle.getGlobalBounds();
            vec2f centerPosValue = {rectangleBounds.left + (rectangleBounds.width - textValueBound.width) / 2.0f,
                    rectangleBounds.top + (rectangleBounds.height - textValueBound.height) / 2.0f};
            vec2f centerPosKey = {rectangleBounds.left + (rectangleBounds.width - textKeyBound.width) / 2.0f,
                    rectangleBounds.top + (rectangleBounds.height - textKeyBound.height) / 2.0f};

            this->keyText.setPosition(centerPosKey.x, centerPosKey.y - 25);
            this->valueText.setPosition(centerPosValue);
            this->hash.setPosition(centerPosValue.x + 25, centerPosValue.y + 25);
            
            this->keyText.setFillColor(key_color);
            this->valueText.setFillColor(value_color);
            this->hash.setFillColor(color::White);
        }

        bool setFont(const char* path_to_string){
            if(!this->m_font.loadFromFile(path_to_string)){
                return false;
            }

            return true;
        }

        void generateTextBox(const vec2f& size, const vec2f& pos, const color& fill_color, const color& outline_color, const float& thickness){
            this->setSize(size).setPosition(pos).setFillColor(fill_color).setOutlineColor(outline_color).setOutlineThickness(thickness);
        }

        bool isEmpty() const{
            return this->height == 0 && this->width == 0;
        }

        sf::RectangleShape getRectangle() const{
            return this->rectangle;
        }

        //Static Functions
        static bool isOverlapping(const TextBox& tbox, const uint16_t& screen_width){
            return true;
        }
};