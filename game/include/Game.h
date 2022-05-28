#pragma once

#include "Resources.h"
#include "World.h"
#include "Hero.h"

#include <SFML/Graphics.hpp>
#include <memory>

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
   void loadResources();

private:
   sf::RenderWindow m_window;
   TextureHolder m_texture_manager;
   World m_world;
   const sf::Time m_time_per_frame;
   std::unique_ptr<Hero> m_hero1;
};