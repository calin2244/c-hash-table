#pragma once
#include "./SFMLTypes.hpp"
#include "./TextBox.hpp"
#include <memory>
using namespace SFMLAliases;

// TODO: Modify constructor: let vec2f position be first, gotta make something uniform

class Button{
    vec2i mousePos;
    TextBox button;
    bool buttonClicked = false;

public:
    Button() = default;
    Button(const TextBox& button): button(button){}
    Button(const vec2f& size, const vec2f& pos, const sf::Font& font, 
            const color& text_color = color::Black, const color& outline = color::Red, const color& fill = color::White) noexcept {
        this->generateButton(size, pos, font, text_color, outline, fill);
    }

    void generateButton(const vec2f& size, const vec2f& pos, const sf::Font& font, 
                        const color& text_color = color::Black, const color& outline = color::Red, const color& fill = color::White) noexcept {
        button.generateTextBox(size, pos, fill, outline, 5);
        button.writeValueText("Insert", &font, text_color);
    }
    
    void renderButtonToScreen(sf::RenderWindow& window) const noexcept{
        this->button.renderToScreen(window);
    }

    // Boolean Functions
    bool is_hovered(const sf::RenderWindow& window, const color& fill_color = color::White) noexcept {
        vec2i mousePosition = mouse::getPosition(window);
        vec2f buttonPosition = this->button.getPosition(); 

        // Convert mouse position to the coordinate system of your button
        vec2f mouseInButtonCoords = window.mapPixelToCoords(mousePosition) - buttonPosition;
        vec2f buttonSize = this->button.getSize();

        bool hovered = mouseInButtonCoords.x >= 0 && 
            mouseInButtonCoords.x <= buttonSize.x &&
            mouseInButtonCoords.y >= 0 && 
            mouseInButtonCoords.y <= buttonSize.y;
        
        hovered ? this->button.setFillColor(fill_color) : this->button.setFillColor(color::White);

        return hovered;
    }

    bool is_clicked(const sf::RenderWindow& window, const color& fill_color = color::White){
        bool hovered = this->is_hovered(window, fill_color);
        
        if(mouse::isButtonPressed(mouse::Left)) {
            if(hovered && !this->buttonClicked) {
                std::cout << "clicked\n";
                this->buttonClicked = true;
                return true;
            }
        }
        else {
            this->buttonClicked = false;
        }

        return false;
    }
};