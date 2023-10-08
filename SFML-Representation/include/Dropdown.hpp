#pragma once
#include "Button.hpp"
#include <vector>
#include <string>

class Dropdown: public Button{
    std::vector<std::string> dropDownOptions;
    std::string activeOption;
    bool isExpanded;
    
public:
    // Dropdown(const vec2f& pos, const vec2f& size, const UIStyle& styleConfig, 
    //          const std::vector<std::string> dropDownOptions = std::vector<std::string>(), FontManager* fontManager, uint16_t characterSize = 16)
    //          : Button(pos, size, styleConfig, dropDownOptions.at[0], fontManager, characterSize), dropDownOptions(dropDownOptions) {}


};