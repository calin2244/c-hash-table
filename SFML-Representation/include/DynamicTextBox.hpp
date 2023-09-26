#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "CustomColors.hpp"

#define CURSOR_BLINK_INTERVAL 0.5f

// TODO: Add a LABEL

class DynamicTextBox{
    sfrectangle text_box;
    sftext text;
    sfclock cursorBlinkTimer;
    sfrectangle cursor;
    std::string input;
    uint16_t limit;
    uint16_t cursorPos;
    bool isCursorVisible;
    bool isActive;

public:
    DynamicTextBox(const vec2f& position, const vec2f& size, sf::Font& font, unsigned int characterSize) noexcept{
        text_box.setPosition(position.x, position.y);
        text_box.setSize(size);
        text_box.setFillColor(color::White);
        text_box.setOutlineColor(color::Black);
        text_box.setOutlineThickness(2.0f);

        text.setFont(font);
        text.setCharacterSize(characterSize);
        text.setFillColor(color::Black);

        // Set the text position relative to the text_box
        text.setPosition(position.x + 5.0f, position.y + size.y / 2.0f - characterSize / 2.0f);

        // Set the cursor as a horizontal bar
        cursor.setSize(vec2f(2.0f, static_cast<float>(characterSize)));
        cursor.setFillColor(color::Black);

        limit = static_cast<uint16_t>(size.x / (characterSize / 2 )) - 1; // Adjust the limit based on the width

        isCursorVisible = true;
        cursorBlinkTimer.restart();
        cursorPos = 0; 
        isActive = false;
    }

    void renderToScreen(sf::RenderWindow& window) noexcept{
        text_box.setOutlineColor(isActive ? CUSTOM_PURPLE : color::Black); 
        text_box.setOutlineThickness(isActive ? 4.0f : 0);           
        window.draw(text_box);
        window.draw(text);

        if(isCursorVisible && isActive){
            window.draw(cursor);
        }
    }

    void setPosition(float x, float y) noexcept{
        text_box.setPosition(x, y);
        text.setPosition(x + 5.0f, y + text_box.getSize().y / 2.0f - text.getCharacterSize() / 2.0f);

        // Adjust the cursor position to be aligned with the text
        cursor.setPosition(text.getPosition().x + cursorPos, text.getPosition().y);
    }


    //? Might delete unicode support, as my C-HashTable implementation doesn't support ATM unicode
    void handleEvent(event event, renderWin& window){
        if(!isActive){
            return; 
        }

        if(event.type == event::TextEntered){
            if(event.text.unicode == '\b' && cursorPos > 0){
                // Handle backspace by removing the character before the cursor
                if(!input.empty()){
                    input.erase(cursorPos - 1, 1);
                    cursorPos--;
                }
            }else if(event.text.unicode < 128 && input.size() < limit){
                // Handle regular characters ASCII
                input.insert(cursorPos, 1, static_cast<char>(event.text.unicode));
                cursorPos++;
            }
            updateText();
        }
    }

    void updateText() noexcept{
        text.setString(input);
        // 4 pixels offset so the bar isn't sticked to the text
        cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 4.0f, text.getPosition().y);
    }

    void setActive(bool active) noexcept{
        this->isActive = active;
    }

    void updateCursorBlink() noexcept{
        if (cursorBlinkTimer.getElapsedTime().asSeconds() >= CURSOR_BLINK_INTERVAL) {
            isCursorVisible = !isCursorVisible;
            cursorBlinkTimer.restart();
        }
    }

    void update(renderWin& window) noexcept{
        updateCursorBlink();
        renderToScreen(window);
    }

    bool containsPoint(float x, float y) const noexcept {
        sf::FloatRect bounds = text_box.getGlobalBounds();
        return bounds.contains(x, y);
    }

    // Getters
    std::string getInputAsString() const noexcept {
        return this->input;
    }

    const char* getInputAsCharArr() const noexcept {
        return this->input.c_str();
    }

    size_t getInputLength() const noexcept {
        return this->input.size();
    }

    bool getIsActive() const noexcept {
        return this->isActive;
    }
};
