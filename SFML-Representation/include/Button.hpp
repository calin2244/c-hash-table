#pragma once
#include "./SFMLTypes.hpp"
#include "./TextBox.hpp"
#include <memory>
using namespace SFMLAliases;

class Button{
    vec2i mousePos;
    TextBox button;
    public:
        Button() = default;
        Button(const TextBox& button): button(button){}
        Button(const vec2f& size, const vec2f& pos, const sf::Font& font) noexcept{
            this->generateButton(size, pos, font);
        }

        void generateButton(const vec2f& size, const vec2f& pos, const sf::Font& font) noexcept{
            button.generateTextBox(size, pos, color::White, color::Red, 5);
            button.writeValueText("Insert", &font, color::Black);
        }
        
        void renderButtonToScreen(sf::RenderWindow& window) const noexcept{
            this->button.renderToScreen(window);
        }
};