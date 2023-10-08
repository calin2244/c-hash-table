#pragma once
#include "./SFMLTypes.hpp"
#include "./FontManager.hpp"
#include <stdint.h>
#include <string>
using namespace SFMLAliases;

class TextBox{
    float height;
    float width;
    sfrectangle rectangle;
    sftext keyText;
    sftext valueText;
    sftext hash;

    protected:
        font m_font;
    
public:
    /*
        * Floats are small enough that passing by value might be faster than by reference
    */
    TextBox(const vec2f& size = {0, 0}) noexcept;
    TextBox(const TextBox& textBox) noexcept;

    TextBox& setOutlineThickness(const float thickness);
    TextBox& setSize(const vec2f& size = {5, 5}) noexcept;
    TextBox& setPosition(const vec2f& pos = {0.0f, 0.0f}) noexcept;
    TextBox& setFillColor(const color& fill_color = color::White) noexcept;
    TextBox& setOutlineColor(const color& outline_color = color::White) noexcept;
    
    virtual void renderToScreen(renderWin& window) const noexcept;
    void writeHashTableText(const std::string_view key, const std::string_view val, 
        const size_t hash, const font& font, 
        const color& key_color = color::White, const color& value_color = color::White, 
        const color& hash_color = color::White) noexcept;
    void writeValueText(const std::string_view valText, const font& font,
                        const color& valColor = color::White, uint16_t characterSize = 16);
    void generateTextBox(const vec2f& size, const vec2f& pos, const color& fill_color = color::Black, 
                        const color& outline_color = color::White, const float& thickness = 2) noexcept;
    
    // Boolean functions
    
    [[nodiscard]] bool isEmpty() const noexcept;

    //Static Functions
    static vec2f getCenterOfRectangle(const sftext& text, const sfrectangle& rectangle) noexcept;

    // Getters
    vec2f getPosition() const noexcept;
    vec2f getSize() const noexcept;
    sfrectangle getRectangle() const noexcept;
};