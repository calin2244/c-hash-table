#pragma once
#include "CustomColors.hpp"
#include "../include/SFMLTypes.hpp"

#define CURSOR_BLINK_INTERVAL 0.5f

using namespace SFMLAliases;

class DynamicTextBox{
    sfrectangle textBox;
    sftext text;
    sftext label;
    sfclock cursorBlinkTimer;
    sfrectangle cursor;
    sprite clearTextSprite;
    texture clearTextSpriteTexture;
    std::string input;
    uint16_t limit;
    uint16_t cursorPos;
    uint16_t characterSize;
    bool isCursorVisible;
    bool isActive;

    void setClearSpriteToEndOfTextBox() noexcept{
        this->clearTextSprite.setPosition(text.getPosition().x + 140, text.getPosition().y);
        this->clearTextSprite.setScale({.06f, .06f});
    }

public:
    DynamicTextBox(const vec2f& position, const vec2f& size, font& font, 
                   uint16_t characterSize, std::string_view texturePath = std::string());

    void renderToScreen(renderWin& window) noexcept;
    void setPosition(float x, float y) noexcept;
    void setLabel(const std::string_view labelName, const UIStyle& styleConfig, const float yOffset = 0.0f) noexcept;
    void handleEvent(event event, renderWin& window);
    void updateText() noexcept;
    void setActive(bool active) noexcept;
    void updateCursorBlink() noexcept;
    void update(renderWin& window) noexcept;
    void clearTextBoxText() noexcept;
    
    bool containsPoint(float x, float y) const noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;

    // Getters
    std::string getInputAsString() const noexcept;
    const char* getInputAsCharArr() const noexcept;
    size_t getInputLength() const noexcept;
    [[nodiscard]] bool getIsActive() const noexcept;
};
