#include "../include/Button.hpp"
#include "../include/SFMLTypes.hpp"
#include "../include/TextBox.hpp"
#include <string>

Button::Button() = default;

Button::Button(const vec2f& pos, const vec2f& size, const UIStyle& styleConfig, 
    const std::string& buttonText, uint16_t characterSize): TextBox(&styleConfig.font){
    this->generateButton(buttonText, size, pos, styleConfig, characterSize);
}

void Button::generateButton(const std::string& buttonText, const vec2f& size, const vec2f& pos, const UIStyle& styleConfig, uint16_t characterSize) noexcept {
    TextBox::generateTextBox(size, pos, styleConfig.fill_color, styleConfig.outline_color, 5);
    TextBox::writeValueText(buttonText, styleConfig.text_color, characterSize);
    this->m_font = &styleConfig.font;
}

void Button::renderToScreen(renderWin& window) const noexcept{
    TextBox::renderToScreen(window);
}

 // Handle event
// Handle mouse button release
void Button::handleMouseButtonReleased(event event, const color& fill_color) {
    if (event.mouseButton.button == mouse::Left && is_hovered(event, fill_color) && buttonClicked) {
        setFillColor(fill_color);
        // Perform any button release actions here
    }
}

// Handle event
void Button::handleEvent(event event, const color& fill_color) {
    if (event.type == event::MouseMoved) {
        handleMouseMoved(event, fill_color);
    }
    else if (event.type == event::MouseButtonPressed) {
        handleMouseButtonPressed(event, fill_color);
    }
    else if (event.type == event::MouseButtonReleased) {
        handleMouseButtonReleased(event, fill_color);
    }
}

// Handle mouse button press
void Button::handleMouseButtonPressed(event event, const color& fill_color) {
    if (event.mouseButton.button == mouse::Left) {
        TextBox::setFillColor(CUSTOM_CYAN);
        if (is_hovered(event, fill_color) && !buttonClicked) {
            buttonClicked = true;
        }
    }
}

// Handle mouse moved (for hover effect)
void Button::handleMouseMoved(event event, const color& fill_color) {
    if (is_hovered(event, fill_color)) {
        // Mouse is over the button
        // Change the fill color to the hover color
        setFillColor(fill_color);
    } else {
        // Mouse is not over the button
        // Change the fill color back to the default color (color::White)
        setFillColor(color::White);
    }
}

bool Button::is_hovered(event event, const color& fill_color) noexcept {
    vec2f mousePosition(event.mouseMove.x, event.mouseMove.y);
    vec2f buttonPosition = getPosition();
    vec2f buttonSize = getSize();

    // Convert mouse position to the coordinate system of your button
    mousePosition -= buttonPosition;

    bool hovered = mousePosition.x >= 0 &&
                    mousePosition.x <= buttonSize.x &&
                    mousePosition.y >= 0 &&
                    mousePosition.y <= buttonSize.y;

    hovered ? setFillColor(fill_color) : setFillColor(color::White);
    return hovered;
}

//Setters
void Button::setText(const std::string_view text, const UIStyle& styleConfig){
    TextBox::writeValueText(text, styleConfig.text_color);
}

// Boolean Functions
bool Button::is_hovered2(const renderWin& window, const color& fill_color) noexcept {
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

bool Button::is_clicked2(const renderWin& window, const color& fill_color) noexcept{
    bool hovered = this->is_hovered2(window, fill_color);
    
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
