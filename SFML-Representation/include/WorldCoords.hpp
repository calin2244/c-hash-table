#pragma once

#include "../SFML/include/SFML/Window/Mouse.hpp"
#include "./SFMLTypes.hpp"
#include <iostream>
#include <functional>
using namespace SFMLAliases;

class WorldCoords{
    std::function<vec2i(void)> worldPos;
    public:
        WorldCoords(){
            worldPos = []() -> vec2i { return sf::Mouse::getPosition(); };
        };

        vec2i getWorldPos() const {
            return this->worldPos();
        }

        static void convertWorldPosToText(const vec2i& pos, sf::Text& text){
            std::string str{"World Pos: "};
            str += std::to_string(pos.x) + ' ' + std::to_string(pos.y);
            text.setString(str);
        }
};
