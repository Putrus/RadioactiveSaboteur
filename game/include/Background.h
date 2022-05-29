#pragma once

#include "Resources.h"
#include <vector>
#include <SFML/Graphics.hpp>

constexpr int background_field_size = 30;
constexpr int background_texture_width = 210;

class Background {
public:
   Background();

   std::pair<int, int> getField(sf::Vector2f position);
   void draw(sf::RenderWindow& window) const;
   void addSprite(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& position);
   void update(sf::Time elapsed_time);
   bool isWater(int x, int y);
   void setColor(int x, int y, const sf::Color& color);
   sf::Color getColor(int x, int y);
   void contaminateArea(int embryo_x, int embryo_y, const sf::Color& contaminate_color);
   void addContaminateSource(int embryo_x, int embryo_y, const sf::Color& contaminate_color, int iterations);

private:
   bool checkField(int x, int y);
   void contaminateArea(int embryo_x, int embryo_y, const sf::Color& contaminate_color, int increment_x, int increment_y);

private:
   struct ContaminateSource
   {
      std::pair<int, int> origin;
      sf::Color color;
      int remaining_cycles;
      float time_since_last_leak;
      //float second_interval;
   };

   std::vector<std::vector<sf::Sprite>> m_sprites;
   std::vector<ContaminateSource> m_contaminate_sources;

   float m_animation_delay;
   float m_animation_speed;
};