#include "../include/FontManager.hpp"

FontManager::FontManager(){
    if(!defaultFont.loadFromFile("../Fonts/JosefinSans-Bold.ttf")){
        defaultFont = sf::Font(); // Reset to a default font like Arial.
    }
}

FontManager& FontManager::getInstance() noexcept{
    static FontManager instance;
    return instance;
}

// Load a font from disk and store it with a unique identifier
bool FontManager::loadFont(std::string_view identifier, std::string_view fontFilePath) noexcept{
    font font;
    if(font.loadFromFile(fontFilePath.data())){
        std::cout << "Loaded " << fontFilePath << " succesfully\n";
        fonts[identifier.data()] = font;
        return true;
    }

    return false;
}

font FontManager::getFont(std::string_view identifier){        
    auto it = fonts.find(identifier.data());
    if(it != fonts.end()){
        std::cout << "Retrieving " << identifier << '\n';
        activeFont = std::move(identifier.data());
        return it->second;
    }

    return defaultFont;
}   

std::string FontManager::getActiveFont() const noexcept{
    return activeFont;
}
