#pragma once

#include <SFML/Graphics.hpp>

class Game {
public:
   Game();
   void run();

private:
   void processEvents();
   void update(sf::Time elapsed_time);
   void render();

private:
   sf::RenderWindow m_window;
   const sf::Time m_time_per_frame;
};