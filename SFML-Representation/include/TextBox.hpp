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
    uint16_t height;
    uint16_t width;
    mutable sf::RectangleShape rectangle;
    mutable sf::Text text;
    sf::Font m_font;
    public:
        TextBox() = default;
        TextBox(const uint16_t& height, const uint16_t& width): height(height), width(width){}

        TextBox& setOutlineThickness(const float& thickness){
            this->rectangle.setOutlineThickness(thickness);
            return *this;
        }

        TextBox& setSize(const vec2f& size = {5, 5}){
            this->rectangle.setSize(size);
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
            window.draw(this->text);
        }

        void writeText(const std::string_view& str, const sf::Font* font, const color& color = color::White){
            sf::FloatRect bounds = rectangle.getGlobalBounds();
            vec2f bnds = {bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f};

            this->text = {str.data(), *font, 18};
           
            sf::FloatRect textBounds = text.getLocalBounds();
            sf::FloatRect rectangleBounds = rectangle.getGlobalBounds();
            vec2f centerPos = {rectangleBounds.left + (rectangleBounds.width - textBounds.width) / 2.0f,
                    rectangleBounds.top + (rectangleBounds.height - textBounds.height) / 2.0f};

            text.setPosition(centerPos);
            text.setFillColor(color);
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

        sf::RectangleShape getRectangle() const{
            return this->rectangle;
        }
};