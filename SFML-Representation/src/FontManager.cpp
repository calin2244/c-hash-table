#include "../include/FontManager.hpp"

FontManager& FontManager::getInstance() noexcept{
    static FontManager instance;
    return instance;
}

// Load a font from disk and store it with a unique identifier
bool FontManager::loadFont(std::string_view identifier, std::string_view fontFilePath) noexcept{
    font font;
    if (font.loadFromFile(fontFilePath.data())) {
        fonts[identifier.data()] = font;
        return true;
    }

    return false;
}

font& FontManager::getFont(std::string_view identifier){        
    auto it = fonts.find(identifier.data());
    if(it != fonts.end())
        return it->second;

    return defaultFont;
}   