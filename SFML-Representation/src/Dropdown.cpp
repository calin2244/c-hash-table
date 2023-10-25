#include "../include/Dropdown.hpp"

Dropdown::Dropdown() = default;

Dropdown::Dropdown(const vec2f& position, const std::vector<std::string>& options, const std::string& defaultOptionText, sf::Font* font)
    : box(vec2f(200, 30)), isExpanded(false), dropdownOptions(options), defaultOptionText(defaultOptionText), font(font) {
    box.setPosition(position);
    box.setOutlineColor(CUSTOM_PURPLE);
    box.setOutlineThickness(2.0f);

    selectedOptionText.setFont(*font);
    selectedOptionText.setCharacterSize(16);
    selectedOptionText.setFillColor(color::Black);
    selectedOptionText.setPosition(position.x + 10, position.y + 5);
    selectedOptionText.setString(defaultOptionText.data());

    // Find the Default Option's Pos
    auto it = std::find(dropdownOptions.begin(), dropdownOptions.end(), defaultOptionText);
    if(it != dropdownOptions.end()){
        selectedOptionIndex = std::distance(dropdownOptions.begin(), it);
    }

     // Create the label text
    labelText.setFont(*font);
    labelText.setCharacterSize(16);
    labelText.setFillColor(color::White);
    labelText.setString("Collision Resolution"); // Set the label text
    labelText.setPosition(position.x, position.y - 25); // Position the label above the dropdown
}

void Dropdown::draw(renderWin& window) {
    // Drawing code (no changes here)
    window.draw(labelText);

    // Always draw the main box
    box.setFillColor(color::White);
    window.draw(box);

    if (isExpanded) {
        // Drawing expanded dropdown options
        sfrectangle optionRect;
        optionRect.setSize(vec2f(box.getSize().x, 30));
        optionRect.setOutlineColor(CUSTOM_PURPLE); // Dark gray outline color
        optionRect.setOutlineThickness(2.5f);

        selectedOptionText.setString(dropdownOptions[selectedOptionIndex]);
        selectedOptionText.setFillColor(color::Black);
        selectedOptionText.setPosition(box.getPosition().x + 10, box.getPosition().y + 5);
        window.draw(selectedOptionText);

        for (size_t i = 0; i < dropdownOptions.size(); ++i) {
            optionRect.setPosition(box.getPosition().x, box.getPosition().y + box.getSize().y + i * 30);

            // Add a condition to change the background color of the hovered option
            if(i == hoveredOptionIndex){
                optionRect.setFillColor(CUSTOM_GRAY); // Light gray background color
            }else{
                optionRect.setFillColor(color(240, 240, 240)); // Default background color
            }

            window.draw(optionRect);

            // Draw the text of the option
            sftext optionText{dropdownOptions[i], *font, 16};
            optionText.setFillColor(color::Black);
            optionText.setPosition(optionRect.getPosition().x + 10, optionRect.getPosition().y + 5);
            window.draw(optionText);

            if(i == selectedOptionIndex){
                optionRect.setFillColor(color(135, 206, 235)); // Highlight the selected option
                // Also, draw the text of the selected option
                sftext selectedOptionText{dropdownOptions[i], *font, 16};
                selectedOptionText.setFillColor(color::Black);
                selectedOptionText.setPosition(optionRect.getPosition().x + 10, optionRect.getPosition().y + 5);
                window.draw(selectedOptionText);
            }
        }
    }else{
        // If not expanded, render only the selected option with hover effect
        window.draw(box); // Render the main box

        // Draw the text of the selected option
        sftext selectedOptionText{dropdownOptions[selectedOptionIndex], *font, 16};
        selectedOptionText.setFillColor(color::Black);
        selectedOptionText.setPosition(box.getPosition().x + 10, box.getPosition().y + 5);

        // Change the background color of the selected option when hovered
        if (hoveredOptionIndex == selectedOptionIndex) {
            box.setFillColor(CUSTOM_GRAY); // Light gray background color
        } else {
            box.setFillColor(color(240, 240, 240)); // Default background color
        }

        window.draw(selectedOptionText);
    }
}

void Dropdown::toggleIsExpanded(){
    isExpanded = !isExpanded;
}

// Event Handling
void Dropdown::handleEvent(event event) {
    switch(event.type){
        case event::MouseButtonPressed: 
            handleMouseButtonPressed(event);
            break;
        case event::MouseMoved: 
            handleMouseMoved(event);
            break;
        default:
            break;
    }
}

void Dropdown::handleMouseButtonPressed(event event) {
    vec2f mousePos(event.mouseButton.x, event.mouseButton.y);

    if (box.getGlobalBounds().contains(mousePos)) {
        isExpanded = !isExpanded; // Toggle the expanded state
    } else if (isExpanded) {
        // Check for option clicks and update the selected option
        rectf optionRectBounds = box.getGlobalBounds();
        optionRectBounds.top += box.getSize().y;
        optionRectBounds.height = 30;

        for (size_t i = 0; i < dropdownOptions.size(); ++i) {
            if (optionRectBounds.contains(mousePos)) {
                selectedOptionIndex = i;
                selectedOptionText.setString(dropdownOptions[i]);
                isExpanded = !isExpanded; // Toggle the expanded state after selecting an option
                break;
            }
            optionRectBounds.top += 30;
        }
    }
}

void Dropdown::handleMouseMoved(event event) {
    if(isExpanded){
        // Handle mouse hover effect over options
        vec2f mousePos(event.mouseMove.x, event.mouseMove.y);

        rectf optionRectBounds = box.getGlobalBounds();
        optionRectBounds.top += box.getSize().y;
        optionRectBounds.height = 30;

        for(size_t i = 0; i < dropdownOptions.size(); ++i){
            if(optionRectBounds.contains(mousePos)){
                hoveredOptionIndex = i;
                break;
            }
            
            optionRectBounds.top += 30;
        }
    }
}
