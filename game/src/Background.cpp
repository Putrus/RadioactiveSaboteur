#include "../include/Background.h"

Background::Background() : m_animation_delay(0.f), m_animation_speed(0.8f) {
}

void Background::draw(sf::RenderWindow& window) const {
   for (auto& sprites : m_sprites) {
      for (auto& sprite : sprites) {
         window.draw(sprite);
      }
   }
}

void Background::addSprite(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& position) {
   sf::Sprite sprite(texture);
   sprite.setTextureRect(rect);
   sprite.setPosition(position);
   if (m_sprites.empty() || m_sprites.back().size() == 64) {
      m_sprites.push_back(std::vector<sf::Sprite>());
   }
   m_sprites.back().push_back(sprite);
}

void Background::update(sf::Time elapsed_time) {
   m_animation_delay += elapsed_time.asSeconds();
   if (m_animation_delay > m_animation_speed) {
      m_animation_delay -= m_animation_speed;
      for (auto& sprites : m_sprites) {
         for (auto& sprite : sprites) {
            sprite.setTextureRect(sf::IntRect((sprite.getTextureRect().left + background_frame_size) % background_texture_width,
               sprite.getTextureRect().top, background_frame_size, background_frame_size));
         }
      }
   }
}