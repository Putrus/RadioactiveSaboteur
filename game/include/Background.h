#pragma once

#include "Resources.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Background {
public:
   void draw(sf::RenderWindow& window) const;
   void addSprite(const sf::Texture& texture, const sf::IntRect& rect);
   void update(sf::Time elapsed_time);

private:
   std::vector<std::vector<sf::Sprite>> m_sprites;
};