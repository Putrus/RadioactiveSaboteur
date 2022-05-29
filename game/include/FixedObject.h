#pragma once

#include <SFML/Graphics.hpp>

class FixedObject
{
public:
   FixedObject(sf::Texture& texture, CollisionItem& collision_item, sf::Vector2f position, sf::Vector2f origin) :
      m_collision_item(collision_item)
   {
      m_sprite.setTexture(texture);
      m_sprite.setOrigin(origin);
      m_sprite.setPosition(position);
   }

   sf::Vector2f getPosition()
   {
      return m_sprite.getPosition();
   }
   void setPosition(const sf::Vector2f& position)
   {
      m_sprite.setPosition(position);
   }

   sf::Sprite& getSprite()
   {
      return m_sprite;
   }
   CollisionItem& getCollisionItem()
   {
      return m_collision_item;
   }

private:
   sf::Sprite m_sprite;
   CollisionItem& m_collision_item;
};
