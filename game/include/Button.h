#pragma once

#include <SFML/Graphics.hpp>

class Button {
public:
   Button(sf::IntRect texture_rect, sf::Texture& texture, sf::Vector2f position);
   sf::Sprite& getSprite();
   void advise(sf::Vector2i& mouse_position);

private:
   bool isAdvisable(sf::Vector2i& mouse_position) const;

private:
   sf::Sprite m_sprite;
};