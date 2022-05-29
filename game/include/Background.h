#pragma once

#include "Resources.h"
#include <vector>
#include <SFML/Graphics.hpp>

constexpr int background_field_size = 30;
constexpr int background_texture_width = 210;

constexpr int background_field_rows = 36;
constexpr int background_field_columns = 64;

class FixedObject;

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
   void addContaminateSource(FixedObject* barrel, int embryo_x, int embryo_y, const sf::Color& contaminate_color, int iterations);
   void updateContaminateSource(FixedObject* barrel, int embryo_x, int embryo_y);
   /// Get number of contaminated fields for given half-part (0: left, 1: right)
   int getContaminationCount(int part);

private:
   bool checkField(int x, int y);
   void contaminateArea(int embryo_x, int embryo_y, const sf::Color& contaminate_color, int increment_x, int increment_y);
   void setFieldState(int x, int y, int state) {
      //if (x < background_field_columns && y < background_field_rows)
      {
         //int index = y * background_field_columns + x;
         //m_field_states[index] = state;
         m_field_states[y][x] = state;
      }
      //else
      //   assert(false);
   }
   int getFieldState(int x, int y) {
      //int index = y * background_field_columns + x;
      //return m_field_states[index];
      return m_field_states[y][x];
   }

private:
   struct ContaminateSource
   {
      FixedObject* barrel;
      std::pair<int, int> origin;
      sf::Color color;
      int remaining_cycles;
      float time_since_last_leak;
      //float second_interval;
   };

   std::vector<std::vector<sf::Sprite>> m_sprites;
   //int m_field_states[background_field_rows * background_field_columns];
   int m_field_states[background_field_rows][background_field_columns];
   std::vector<ContaminateSource> m_contaminate_sources;

   float m_animation_delay;
   float m_animation_speed;
};