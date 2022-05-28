#pragma once

#include "World.h"
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
   World m_world;
   const sf::Time m_time_per_frame;
};