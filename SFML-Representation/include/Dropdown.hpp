#pragma once
#include "Button.hpp"
#include <vector>
#include <string>

class Dropdown: public Button{
    std::vector<std::string> dropDownOptions;
    bool isExpanded;
    
public:
    Dropdown(const vec2f& pos, const vec2f& size, const UIStyle& styleConfig, 
             const std::vector<std::string> dropDownOptions = std::vector<std::string>())
             : Button(pos, size, styleConfig), dropDownOptions(dropDownOptions) {}

};