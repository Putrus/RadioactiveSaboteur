#include "../include/Background.h"



void Background::draw(sf::RenderWindow& window) const {
   for (auto& sprites : m_sprites) {
      for (auto& sprite : sprites) {
         window.draw(sprite);
      }
   }
}

void Background::addSprite(const sf::Texture& texture, const sf::IntRect& rect) {
   sf::Sprite sprite(texture);
   sprite.setTextureRect(rect);
   if (m_sprites.empty() || m_sprites.back().size() == 64) {
      m_sprites.push_back(std::vector<sf::Sprite>());
   }
   m_sprites.back().push_back(sprite);
}