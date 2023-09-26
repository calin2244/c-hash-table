#pragma once
#include "Button.hpp"
#include <vector>

class Dropdown: public Button{
    std::vector<string> dropDownOptions;
    bool isExpanded;
    
public:
    Dropdown(const vec2f& pos, const vec2f& size, const UIStyle& styleConfig, 
             const std::vector<string> dropDownOptions = std::vector<string>())
             : Button(pos, size, styleConfig), dropDownOptions(dropDownOptions) {}
};