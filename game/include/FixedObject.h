#pragma once

#include <SFML/Graphics.hpp>

class FixedObject
{
public:
   FixedObject(sf::Texture& texture, sf::Vector2f position, sf::Vector2f origin)
   {
      m_sprite.setTexture(texture);
      m_sprite.setOrigin(origin);
      m_sprite.setPosition(position);
   }

   //void setPosition(const sf::Vector2f& position)
   //{
   //   m_sprite.setPosition(position);
   //}

   sf::Sprite& getSprite()
   {
      return m_sprite;
   }

private:
   sf::Sprite m_sprite;
};
