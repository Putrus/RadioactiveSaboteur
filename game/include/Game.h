#pragma once

#include "ResourceHolder.h"
#include "World.h"
#include <SFML/Graphics.hpp>

class Game {
public:
   Game();
   void run();
   void newGame();

private:
   void processEvents();
   void update(sf::Time elapsed_time);
   void render();
   void reportError(const std::string& msg);

private:
   sf::RenderWindow m_window;
   ResourceHolder<sf::Texture, std::string> m_texture_manager;
   World m_world;
   const sf::Time m_time_per_frame;
};