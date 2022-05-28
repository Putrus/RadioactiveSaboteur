#pragma once

#include "Background.h"
#include "Resources.h"
#include "World.h"
#include "Hero.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Game {
public:
   Game(const std::string& root_path);
   void run();
   void newGame();

private:
   void processEvents();
   void update(sf::Time elapsed_time);
   void render();
   void reportError(const std::string& msg);
   void loadResources();
   void processPlayerEvents(Hero& hero, const sf::Event& event, bool player);

private:
   std::string m_root_path;
   sf::RenderWindow m_window;
   TextureHolder m_texture_manager;
   std::vector<const sf::Drawable*> g_renderables;
   World m_world;
   Background m_background;
   const sf::Time m_time_per_frame;
   std::unique_ptr<Hero> bomba;
   std::unique_ptr<Hero> kurwinox;
};