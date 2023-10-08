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
    using sprite = sf::Sprite;
    using texture = sf::Texture;
}

// TODO: Add the character size here

struct UIStyle{
    sf::Font& font;
    sf::Color text_color = sf::Color::Black;
    sf::Color outline_color = sf::Color::Red;
    sf::Color fill_color = sf::Color::White;

    UIStyle(sf::Font& font, 
           const sf::Color& text_c = sf::Color::Black,
           const sf::Color& fill_c = sf::Color::White,
           const sf::Color& out_c = sf::Color::Red): 
           font(font), text_color(text_c), fill_color(fill_c), outline_color(out_c) {}
};