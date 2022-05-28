#pragma once

#include <SFML/Graphics.hpp>

constexpr int frame_size = 64;
constexpr int texture_width = 256;
constexpr int texture_height = 256;
constexpr float hero_speed = 5.0f;

enum Action {
   IDLE = 0,
   DOWN,
   RIGHT,
   LEFT,
   UP,
   UPLEFT,
   UPRIGHT,
   DOWNLEFT,
   DOWNRIGHT
};

struct CollisionItem;

class Hero
{
public:
   Hero(sf::Texture& texture, CollisionItem &collision_item, sf::Vector2f position);

   float getRadius() const
   {
      return m_radius;
   }
   sf::Vector2f getPosition() const;
   void setPosition(const sf::Vector2f& position);
   sf::Sprite& getSprite();
   void setAction(Action action);
   void update(sf::Time elapsed_time);
   sf::Vector2f getSpeed() const;

private:
   sf::Sprite m_sprite;
   float m_radius;
   //CollisionItem& m_collision_item;
   sf::Vector2f m_speed;
   Action m_action;
   sf::Time m_last_animation_frame_time;
   
   float m_animation_delay;
   float m_animation_speed;
};