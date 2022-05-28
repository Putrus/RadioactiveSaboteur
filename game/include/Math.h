#pragma once

#include <SFML/System.hpp>
#include <cassert>

namespace math
{
   constexpr float SQRT_2 = 1.41f;
   constexpr float PI = 3.14f;

   inline float dot_product(sf::Vector2f v1, sf::Vector2f v2)
   {
      return v1.x * v2.x + v1.y + v2.y;
   }

   inline float length(sf::Vector2f v)
   {
      return sqrt(v.x * v.x + v.y + v.y);
   }

   inline float distance(sf::Vector2f v1, sf::Vector2f v2)
   {
      sf::Vector2f v = v2 - v1;
      return length(v);
   }

   inline void set_length(sf::Vector2f &v, float len)
   {
      assert(len != 0);
      if (len == 0)
         return;

      float cur_len = length(v);
      v *= len / cur_len;
   }
}
