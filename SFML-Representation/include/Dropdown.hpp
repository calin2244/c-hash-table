#pragma once
#include "../include/SFMLTypes.hpp"
#include "../include/CustomColors.hpp"
#include <vector>

using namespace SFMLAliases;

class Dropdown {
public:
    Dropdown();
    Dropdown(const vec2f& position, const std::vector<std::string>& options, const std::string& defaultOptionText, sf::Font* font);
    void draw(renderWin& window);
    void toggleIsExpanded();
    void handleEvent(event event);
    void handleMouseButtonPressed(event event);
    void handleMouseMoved(event event);

private:
    sfrectangle box;
    sftext selectedOptionText;
    sftext labelText;
    font* font;
    bool isExpanded;
    std::vector<std::string> dropdownOptions;
    std::string_view defaultOptionText;
    size_t selectedOptionIndex;
    size_t hoveredOptionIndex;
};
