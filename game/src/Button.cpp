#include "../include/Button.h"

Button::Button(sf::IntRect texture_rect, sf::Texture& texture, sf::Vector2f position) : m_sprite(texture) {
   m_sprite.setTextureRect(texture_rect);
   m_sprite.setPosition(position);
   m_sprite.setColor(sf::Color(100, 100, 100));
}

bool Button::isAdvisable(const sf::Vector2i& mouse_position) const {
   sf::FloatRect rect = m_sprite.getGlobalBounds();
   return mouse_position.x > rect.left && mouse_position.x < (rect.left + rect.width) && mouse_position.y > rect.top && mouse_position.y < (rect.top + rect.height);
}

sf::Sprite& Button::getSprite() {
   return m_sprite;
}

void Button::advise(const sf::Vector2i& mouse_position) {
   if (isAdvisable(mouse_position)) {
      m_sprite.setColor(sf::Color(255, 255, 255));
   }
   else {
      m_sprite.setColor(sf::Color(100, 100, 100));
   }
}