#pragma once
#include "./SFMLTypes.hpp"
#include <unordered_map>
#include <string>
using namespace SFMLAliases;

class FontManager{
    std::unordered_map<std::string, font> fonts;
    font defaultFont;
    FontManager(){
       if(!defaultFont.loadFromFile("../Fonts/JosefinSans-Bold.ttf")){
            defaultFont = sf::Font(); // Reset to a default font like Arial.
        }
    }
    
public:
    static FontManager& getInstance() noexcept;

    // Load a font from disk and store it with a unique identifier
    bool loadFont(std::string_view identifier, std::string_view fontFilePath) noexcept;
    font& getFont(std::string_view identifier);
};
