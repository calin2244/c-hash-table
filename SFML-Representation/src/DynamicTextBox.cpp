#include "../include/DynamicTextBox.hpp"

DynamicTextBox::DynamicTextBox(const vec2f& position, const vec2f& size, font& font, 
               uint16_t characterSize, std::string_view texturePath): characterSize(characterSize){
    this->textBox.setPosition(position.x, position.y);
    this->textBox.setSize(size);
    this->textBox.setFillColor(color::White);
    this->textBox.setOutlineColor(color::Black);
    this->textBox.setOutlineThickness(2.0f);

    this->text.setFont(font);
    this->text.setCharacterSize(characterSize);
    this->text.setFillColor(color::Black);

    // Set the text position relative to the textBox
    this->text.setPosition(position.x + 5.0f, position.y + size.y / 2.0f - characterSize / 2.0f);

    // Set the cursor as a horizontal bar
    this->cursor.setSize(vec2f(2.0f, static_cast<float>(characterSize)));
    this->cursor.setFillColor(color::Black);

    this->limit = static_cast<uint16_t>(size.x / (characterSize / 2 )) - 1; // Adjust the limit based on the width

    if(this->clearTextSpriteTexture.loadFromFile(texturePath.data())){
        this->clearTextSprite.setTexture(clearTextSpriteTexture);
        setClearSpriteToEndOfTextBox();
    }

    this->isCursorVisible = true;
    this->cursorBlinkTimer.restart();
    this->cursorPos = 0; 
    this->isActive = false;
}

void DynamicTextBox::renderToScreen(renderWin& window) noexcept{
    this->textBox.setOutlineColor(isActive ? CUSTOM_PURPLE : color::Black); 
    this->textBox.setOutlineThickness(isActive ? 4.0f : 0);           
    window.draw(this->textBox);
    window.draw(this->text);
    window.draw(this->label);
    window.draw(this->clearTextSprite);

    if(this->isCursorVisible && this->isActive){
        window.draw(this->cursor);
    }
}

void DynamicTextBox::setPosition(float x, float y) noexcept{
    this->textBox.setPosition(x, y);
    this->text.setPosition(x + 5.0f, y + textBox.getSize().y / 2.0f - text.getCharacterSize() / 2.0f);

    // Adjust the cursor position to be aligned with the text
    this->cursor.setPosition(text.getPosition().x + cursorPos, text.getPosition().y);
}

void DynamicTextBox::setLabel(const std::string_view labelName, const UIStyle& styleConfig, const float yOffset) noexcept{
    this->label = { labelName.data(), styleConfig.font };
    this->label.setCharacterSize(this->characterSize / 1.6f);
    this->label.setFillColor(styleConfig.fill_color);
    this->label.setOutlineColor(styleConfig.outline_color);
    sf::FloatRect textBoxBounds = textBox.getGlobalBounds();
    this->label.setPosition(textBoxBounds.left, textBoxBounds.top - yOffset);
}


//? Might delete unicode support, as my C-HashTable implementation doesn't support ATM unicode
void DynamicTextBox::handleEvent(event event, renderWin& window){
    if(!this->isActive){
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

void DynamicTextBox::updateText() noexcept{
    this->text.setString(input);
    // 4 pixels offset so the bar isn't sticked to the text
    this->cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 4.0f, text.getPosition().y);
}

void DynamicTextBox::setActive(bool active) noexcept{
    this->isActive = active;
}

void DynamicTextBox::updateCursorBlink() noexcept{
    if (cursorBlinkTimer.getElapsedTime().asSeconds() >= CURSOR_BLINK_INTERVAL) {
        isCursorVisible = !isCursorVisible;
        cursorBlinkTimer.restart();
    }
}

void DynamicTextBox::update(renderWin& window) noexcept{
    updateCursorBlink();
    renderToScreen(window);
}

void DynamicTextBox::clearTextBoxText() noexcept{
    this->input.clear();
    this->text.setString("");
    this->cursorPos = 0;
    this->cursor.setPosition(text.getPosition());
}

bool DynamicTextBox::containsPoint(float x, float y) const noexcept {
    sf::FloatRect bounds = textBox.getGlobalBounds();
    return bounds.contains(x, y);
}

[[nodiscard]] bool DynamicTextBox::isEmpty() const noexcept{
    return std::empty(this->input);
}

// Getters
std::string DynamicTextBox::getInputAsString() const noexcept {
    return this->input;
}

const char* DynamicTextBox::getInputAsCharArr() const noexcept {
    return this->input.c_str();
}

size_t DynamicTextBox::getInputLength() const noexcept {
    return this->input.size();
}

[[nodiscard]]
bool DynamicTextBox::getIsActive() const noexcept {
    return this->isActive;
}