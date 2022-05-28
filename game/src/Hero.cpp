#include "../include/Hero.h"

#define SQRT2 1.41f

Hero::Hero(sf::Texture& texture, /*CollisionItem &collision_item, */sf::Vector2f position) : 
   m_sprite(texture), m_speed(0.f, 0.f), m_action(Action::IDLE), m_animation_delay(0.f), m_animation_speed(0.2f) {
   // idle - [0,0]
   sf::IntRect rect(0, 0, frame_size, frame_size);
   m_sprite.setTextureRect(rect);
   m_sprite.setOrigin(frame_size / 2, frame_size / 2);
   m_sprite.setPosition(position);
}

void Hero::setPosition(const sf::Vector2f& position)
{
   m_sprite.setPosition(position);
}

sf::Sprite& Hero::getSprite()
{
   return m_sprite;
}

void Hero::setAction(Action action) {
   m_action = action;
   switch (action) {
   case Action::IDLE:
   {
      m_speed = sf::Vector2f(0.0f, 0.0f);
      break;
   }
   case Action::DOWN:
   {
      m_speed = sf::Vector2f(0.0f, hero_speed);
      break;
   }
   case Action::RIGHT:
   {
      m_speed = sf::Vector2f(hero_speed, 0.0f);
      break;
   }
   case Action::LEFT:
   {
      m_speed = sf::Vector2f(-hero_speed, 0.0f);
      break;
   }
   case Action::UP:
   {
      m_speed = sf::Vector2f(0.0f, -hero_speed);
      break;
   }
   case Action::UPLEFT:
   {
      m_speed = sf::Vector2f(-hero_speed / SQRT2, -hero_speed / SQRT2);
      m_action = UP;
      break;
   }
   case Action::UPRIGHT:
   {
      m_speed = sf::Vector2f(hero_speed / SQRT2, -hero_speed / SQRT2);
      m_action = UP;
      break;
   }
   case Action::DOWNLEFT:
   {
      m_speed = sf::Vector2f(-hero_speed / SQRT2, hero_speed / SQRT2);
      m_action = DOWN;
      break;
   }
   case Action::DOWNRIGHT:
   {
      m_speed = sf::Vector2f(hero_speed / SQRT2, hero_speed / SQRT2);
      m_action = DOWN;
      break;
   }
   }
}

void Hero::update(sf::Time elapsed_time) {
   //update position
   m_sprite.setPosition(m_sprite.getPosition() + m_speed);
   
   //update animation
   m_animation_delay += elapsed_time.asSeconds();
   if (m_animation_delay > m_animation_speed) {
      m_animation_delay -= m_animation_speed;
      m_sprite.setTextureRect(sf::IntRect((m_sprite.getTextureRect().left + 64) % texture_width, 
         static_cast<int>(m_action) * 64, frame_size, frame_size));

   }
}

sf::Vector2f Hero::getSpeed() const {
   return m_speed;
}