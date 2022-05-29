#pragma once

#include <SFML/Graphics.hpp>

class Button {
public:
   Button(sf::IntRect texture_rect, sf::Texture& texture, sf::Vector2f position);
   sf::Sprite& getSprite();
   void advise(const sf::Vector2i& mouse_position);

private:
   bool isAdvisable(const sf::Vector2i& mouse_position) const;

private:
   sf::Sprite m_sprite;
};