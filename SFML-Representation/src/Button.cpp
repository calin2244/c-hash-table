#include "../include/Button.hpp"
#include "../include/SFMLTypes.hpp"
#include "../include/TextBox.hpp"
#include <string>

Button::Button() = default;

Button::Button(const vec2f& pos, const vec2f& size, const UIStyle& styleConfig, 
    const std::string& buttonText, uint16_t characterSize): TextBox(size){
    this->generateButton(buttonText, size, pos, styleConfig, characterSize);
}

void Button::generateButton(const std::string& buttonText, const vec2f& size, const vec2f& pos, const UIStyle& styleConfig, uint16_t characterSize) noexcept {
    TextBox::generateTextBox(size, pos, styleConfig.fill_color, styleConfig.outline_color, 5);
    TextBox::writeValueText(buttonText, styleConfig.font, styleConfig.text_color, characterSize);
    this->m_font = styleConfig.font;
}

void Button::renderToScreen(renderWin& window) const noexcept{
    TextBox::renderToScreen(window);
}

// Boolean Functions
bool Button::is_hovered(const renderWin& window, const color& fill_color) noexcept {
    vec2i mousePosition = mouse::getPosition(window);
    vec2f buttonPosition = TextBox::getPosition(); 

    // Convert mouse position to the coordinate system of your button
    vec2f mouseInButtonCoords = window.mapPixelToCoords(mousePosition) - buttonPosition;
    vec2f buttonSize = TextBox::getSize();

    bool hovered = mouseInButtonCoords.x >= 0 && 
        mouseInButtonCoords.x <= buttonSize.x &&
        mouseInButtonCoords.y >= 0 && 
        mouseInButtonCoords.y <= buttonSize.y;
    
    hovered ? TextBox::setFillColor(fill_color) : TextBox::setFillColor(color::White);

    return hovered;
}

bool Button::is_clicked(const renderWin& window, const color& fill_color) noexcept{
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
void Button::setText(const std::string_view text, const UIStyle& styleConfig){
    TextBox::writeValueText(text, styleConfig.font, styleConfig.text_color);
}