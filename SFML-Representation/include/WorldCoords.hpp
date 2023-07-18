#pragma once

#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Window/Mouse.hpp"
#include <iostream>
#include <functional>

class WorldCoords{
    std::function<sf::Vector2<int>(void)> worldPos;
    public:
        WorldCoords(){
            worldPos = []() -> sf::Vector2<int> { return sf::Mouse::getPosition(); };
        };

        sf::Vector2<int> getWorldPos() const {
            return this->worldPos();
        }

        static void convertWorldPosToText(const sf::Vector2<int>& pos, sf::Text& text){
            std::string str{"World Pos: "};
            str += std::to_string(pos.x) + ' ' + std::to_string(pos.y);
            text.setString(str);
        }
};
