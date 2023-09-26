#pragma once
#include "../SFML/include/SFML/Graphics.hpp"

namespace SFMLAliases{
    using vec2f = sf::Vector2<float>;
    using vec2i = sf::Vector2<int>;
    using color = sf::Color;
    using sftext = sf::Text;
    using sfrectangle = sf::RectangleShape;
    using recti = sf::Rect<int>;
    using rectf = sf::Rect<float>;
    using mouse = sf::Mouse;
    using event = sf::Event;
    using font = sf::Font;
    using sfclock = sf::Clock;
    using renderWin = sf::RenderWindow;
    using sfview = sf::View;
}

struct UIStyle{
    sf::Font font;
    sf::Color text_color = sf::Color::Black;
    sf::Color outline_color = sf::Color::Red;
    sf::Color fill_color = sf::Color::White;
};