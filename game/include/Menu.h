#pragma once

#include "Button.h"

class Menu {
public:
   Menu(sf::Texture& texture);
   void draw(sf::RenderWindow& window);
   void update(const sf::Vector2i& mouse_position);
   bool isVisible();
   void setVisible(bool visible);
   unsigned char click(const sf::Vector2i& mouse_position);

private:
   std::unique_ptr<Button> m_start;
   std::unique_ptr<Button> m_credits;
   std::unique_ptr<Button> m_exit;
   bool m_is_visible;
};