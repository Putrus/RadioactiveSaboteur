#pragma once

#include "Resources.h"
#include <vector>
#include <SFML/Graphics.hpp>

constexpr int background_frame_size = 64;
constexpr int background_texture_width = 210;

class Background {
public:
   Background();
   void draw(sf::RenderWindow& window) const;
   void addSprite(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& position);
   void update(sf::Time elapsed_time);

private:
   std::vector<std::vector<sf::Sprite>> m_sprites;

   float m_animation_delay;
   float m_animation_speed;
};