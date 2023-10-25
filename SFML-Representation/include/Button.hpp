#pragma once
#include "./SFMLTypes.hpp"
#include "./TextBox.hpp"
#include "CustomColors.hpp"
using namespace SFMLAliases;

// * Now class inherits from TextBox :P

class Button: public TextBox{
    vec2i mousePos;
    bool buttonClicked = false;

public:
    Button();
    Button(const vec2f& pos, const vec2f& size, const UIStyle& styleConfig, 
           const std::string& buttonText = std::string(), uint16_t characterSize = 16);

    void generateButton(const std::string& buttonText, const vec2f& size, const vec2f& pos, const UIStyle& styleConfig, uint16_t characterSize = 16) noexcept;    
    void renderToScreen(renderWin& window) const noexcept override;
    // Boolean Functions
    bool is_hovered(event event, const color& fill_color = color::White) noexcept;
    void handleEvent(event event, const color& fill_color = color::White);
    void handleMouseMoved(event event, const color& fill_color);
    void handleMouseButtonPressed(event event, const color& fill_color);
    void handleMouseButtonReleased(event event, const color& fill_color);

    //Setters
    void setText(const std::string_view text, const UIStyle& styleConfig);

    // OLD FUNCS
    bool is_hovered2(const renderWin& window, const color& fill_color = color::White) noexcept;
    bool is_clicked2(const renderWin& window, const color& fill_color = color::White) noexcept;
};