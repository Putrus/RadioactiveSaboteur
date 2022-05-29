#include "../include/Background.h"

Background::Background() : m_animation_delay(0.f), m_animation_speed(0.2f) {
}

std::pair<int, int> Background::getField(sf::Vector2f position) {
   int first = position.x / background_field_size;
   int second = position.y / background_field_size;
   return std::make_pair(first, second);
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
   float elapsed_seconds = elapsed_time.asSeconds();
   m_animation_delay += elapsed_seconds;
   if (m_animation_delay > m_animation_speed) {
      m_animation_delay -= m_animation_speed;
      for (auto& sprites : m_sprites) {
         for (auto& sprite : sprites) {
            if (sprite.getTextureRect().top > 0) {
               sf::IntRect rect = sprite.getTextureRect();
               sprite.setTextureRect(sf::IntRect((sprite.getTextureRect().left + background_field_size) % background_texture_width,
                  sprite.getTextureRect().top, background_field_size, background_field_size));
            }
         }
      }
   }

   // comtaminate
   constexpr float COMTAMINATE_SECOND_INTERVAL = 10;
   for (auto it = m_contaminate_sources.begin(); it != m_contaminate_sources.end();)
   {
      ContaminateSource& source = *it;
      source.time_since_last_leak += elapsed_seconds;
      if (source.time_since_last_leak > COMTAMINATE_SECOND_INTERVAL)
      {
         contaminateArea(source.origin.first, source.origin.second, source.color);
         source.time_since_last_leak -= COMTAMINATE_SECOND_INTERVAL;
         --source.remaining_cycles;

         if (source.remaining_cycles <= 0)
         {
            it = m_contaminate_sources.erase(it);
         }
         else
            ++it;
      }
      else
         ++it;
   }
}

bool Background::isWater(int x, int y) {
   if (checkField(x, y)) {
      return m_sprites[y][x].getTextureRect().top > 0;
   }
   return false;
}

void Background::setColor(int x, int y, const sf::Color& color) {
   if (checkField(x, y)) {
      m_sprites[y][x].setColor(color);
   }
}
sf::Color Background::getColor(int x, int y) {
   if (checkField(x, y)) {
      return m_sprites[y][x].getColor();
   }
   return sf::Color(0, 0, 0);
}

bool Background::checkField(int x, int y) {
   if (m_sprites.size() > y && m_sprites[y].size() > x) {
      return true;
   }
   return false;
}

void Background::contaminateArea(int embryo_x, int embryo_y, const sf::Color& contaminate_color) {
   contaminateArea(embryo_x, embryo_y, contaminate_color, -1, 0);
   contaminateArea(embryo_x, embryo_y, contaminate_color, 1, 0);
   contaminateArea(embryo_x, embryo_y, contaminate_color, 0, -1);
   contaminateArea(embryo_x, embryo_y, contaminate_color, 0, 1);
}

void Background::contaminateArea(int embryo_x, int embryo_y, const sf::Color& contaminate_color, int increment_x, int increment_y) {
   int x = embryo_x;
   int y = embryo_y;
   while (getColor(x, y) == contaminate_color) {
      x += increment_x;
      y += increment_y;
   }
   setColor(x, y, contaminate_color);
   setColor(x - increment_y, y - increment_x, contaminate_color);
   setColor(x + increment_y, y + increment_x, contaminate_color);
}

void Background::addContaminateSource(int embryo_x, int embryo_y, const sf::Color& contaminate_color, int iterations)
{
   ContaminateSource source;
   source.origin = std::make_pair(embryo_x, embryo_y);
   source.color = contaminate_color;
   source.remaining_cycles = iterations;
   //source.second_interval = ;
   source.time_since_last_leak = 0;
   m_contaminate_sources.emplace_back(source);
}
