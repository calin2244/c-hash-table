#pragma once
#include "./SFMLTypes.hpp"
#include "./TextBox.hpp"
#include <memory>
#include <string>
using namespace SFMLAliases;

// TODO?: Consider inheriting from TextBox to allow move somantics in the set text area and reusability of functions

class Button{
    vec2i mousePos;
    TextBox button;
    bool buttonClicked = false;

public:
    Button() = default;
    Button(const TextBox& button): button(button){}
    Button(const vec2f& pos, const vec2f& size, const UIStyle& styleConfig, const std::string& buttonText = std::string()){
        this->generateButton(buttonText, size, pos, styleConfig);
    }

    void generateButton(const std::string& buttonText, const vec2f& size, const vec2f& pos, const UIStyle& styleConfig) noexcept {
        button.generateTextBox(size, pos, styleConfig.fill_color, styleConfig.outline_color, 5);
        button.writeValueText(buttonText, &styleConfig.font, styleConfig.text_color);
    }
    
    void renderToScreen(sf::RenderWindow& window) const noexcept{
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

    bool is_clicked(const sf::RenderWindow& window, const color& fill_color = color::White) noexcept{
        bool hovered = this->is_hovered(window, fill_color);
        
        if(mouse::isButtonPressed(mouse::Left)) {
            if(hovered && !this->buttonClicked) {
                // std::cout << "clicked\n";
                this->buttonClicked = true;
                return true;
            }
        }
        else {
            this->buttonClicked = false;
        }

        return false;
    }

    //Setters
    void setText(const std::string_view text, const UIStyle& styleConfig){
        button.writeValueText(text, &styleConfig.font, styleConfig.text_color);
    }
};