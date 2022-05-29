#include "../include/Hero.h"
#include "../include/FixedObject.h"
#include "../include/World.h"

#define SQRT2 1.41f
constexpr float BACKBACK_VERTICAL_OFFSET = 15;

Hero::Hero(sf::Texture& texture, CollisionItem& collision_item, sf::Vector2f position) :
   m_sprite(texture),
   m_collision_item(collision_item),
   m_backpack_object(nullptr),
   m_speed(0.f, 0.f),
   m_action(Action::IDLE),
   m_animation_delay(0.f),
   m_animation_speed(0.2f),
   m_water(false),
   m_hero_speed(4.0f)
{
   // idle - [0,0]
   sf::IntRect rect(0, 0, frame_size, frame_size);
   m_sprite.setTextureRect(rect);
   // todo!!!!!!!!!!!!!!!!!!!!
   m_sprite.setOrigin(32, 52);
   //m_sprite.setOrigin(frame_size / 2, frame_size / 2);
   m_sprite.setPosition(position);

   // todo ugly like a hell
   m_radius = collision_item.shape.sphere.radius;
}

sf::Vector2f Hero::getPosition() const
{
   return m_sprite.getPosition();
}

void Hero::setPosition(const sf::Vector2f& position)
{
   m_sprite.setPosition(position);
   //offset_aabb(m_collision_item.shape.aabb, offset);
   m_collision_item.shape.aabb.left = position.x - 16;
   m_collision_item.shape.aabb.right = position.x + 16;
   m_collision_item.shape.aabb.top = position.y - 16;
   m_collision_item.shape.aabb.bottom = position.y + 16;

   if (m_backpack_object)
   {
      m_backpack_object->setPosition(position - sf::Vector2f(0, BACKBACK_VERTICAL_OFFSET));
   }
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
      m_speed = sf::Vector2f(0.0f, m_hero_speed);
      break;
   }
   case Action::RIGHT:
   {
      m_speed = sf::Vector2f(m_hero_speed, 0.0f);
      break;
   }
   case Action::LEFT:
   {
      m_speed = sf::Vector2f(-m_hero_speed, 0.0f);
      break;
   }
   case Action::UP:
   {
      m_speed = sf::Vector2f(0.0f, -m_hero_speed);
      break;
   }
   case Action::UPLEFT:
   {
      m_speed = sf::Vector2f(-m_hero_speed / SQRT2, -m_hero_speed / SQRT2);
      m_action = UP;
      break;
   }
   case Action::UPRIGHT:
   {
      m_speed = sf::Vector2f(m_hero_speed / SQRT2, -m_hero_speed / SQRT2);
      m_action = UP;
      break;
   }
   case Action::DOWNLEFT:
   {
      m_speed = sf::Vector2f(-m_hero_speed / SQRT2, m_hero_speed / SQRT2);
      m_action = DOWN;
      break;
   }
   case Action::DOWNRIGHT:
   {
      m_speed = sf::Vector2f(m_hero_speed / SQRT2, m_hero_speed / SQRT2);
      m_action = DOWN;
      break;
   }
   }
}

void Hero::update(sf::Time elapsed_time) {
   //update position
   //m_sprite.setPosition(m_sprite.getPosition() + m_speed);
   
   //update animation
   m_animation_delay += elapsed_time.asSeconds();
   if (m_animation_delay > m_animation_speed) {
      m_animation_delay -= m_animation_speed;
      m_sprite.setTextureRect(sf::IntRect((m_sprite.getTextureRect().left + 64) % texture_width, 
         static_cast<int>(m_action) * 64 + (m_water ? (m_action == Action::IDLE ? 320 : 256) : 0), frame_size, frame_size));

   }
}

sf::Vector2f Hero::getSpeed() const {
   return m_speed;
}

void Hero::barrelToBackpack(FixedObject* barrel)
{
   barrel->getSprite().setScale(0.75f, 0.75f);
   m_backpack_object = barrel;
}

FixedObject* Hero::dropBarrel()
{
   if (!m_backpack_object)
      return nullptr;

   m_backpack_object->getSprite().setScale(1, 1);
   m_backpack_object->setPosition(m_backpack_object->getPosition() + sf::Vector2f(0, BACKBACK_VERTICAL_OFFSET));
   FixedObject* barrel = m_backpack_object;
   m_backpack_object = nullptr;
   return barrel;
}

bool Hero::isBackpackEmpty()
{
   return m_backpack_object == nullptr;
}

void Hero::setWater(bool water) {
   m_hero_speed = water ? 2.0f : 4.0f;
   m_water = water;
}