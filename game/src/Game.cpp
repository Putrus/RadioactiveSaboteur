#include "../include/Game.h"

#include <SFML/Graphics.hpp>
#include <cassert>

Game::Game(const std::string& root_path) :
   m_root_path(root_path),
   m_window(sf::VideoMode(400, 400), "Radioactive Saboteur"), m_time_per_frame(sf::seconds(1.f / 60.f))
{
   if (m_root_path.size() > 0)
   {
      if (m_root_path.back() != '/' && m_root_path.back() != '\\')
         m_root_path += '\\';
   }
}

void Game::run() {
   sf::Clock clock;
   sf::Time time_since_last_update = sf::Time::Zero;
   // todo connect with menu handler at the end!
   newGame();

   //game loop
   while (m_window.isOpen()) {
      sf::Time elapsed_time = clock.restart();
      time_since_last_update += elapsed_time;
      while (time_since_last_update > m_time_per_frame) {
         time_since_last_update -= m_time_per_frame;

         processEvents();
         update(m_time_per_frame);
      }
      render();
   }
}


constexpr int frame_size = 64;
constexpr int texture_width = 256;
constexpr int texture_height = 1024;

class Hero
{
public:
   Hero(sf::Texture& texture, /*CollisionItem &collision_item, */sf::Vector2f position) :
      m_texture(texture)
   {
      // idle - [0,0]
      sf::IntRect rect(0, 0, frame_size, frame_size);
      m_sprite.setTexture(texture);
      m_sprite.setTextureRect(rect);
      m_sprite.setOrigin(frame_size / 2, frame_size / 2);
      m_sprite.setPosition(position);
   }

   void setPosition(const sf::Vector2f& position)
   {
      m_sprite.setPosition(position);
   }

   sf::Sprite& getSprite()
   {
      return m_sprite;
   }

private:
   sf::Texture& m_texture;
   sf::Sprite m_sprite;
};

Hero* g_hero1;

void Game::newGame()
{
   std::string data_path = m_root_path + "data\\";
   sf::Texture *texture = m_texture_manager.load("Hero1", (data_path + "hero1.png").c_str());
   if (!texture)
   {
      reportError("Cannot load texture");
      return;
   }

   sf::Vector2f position1(50, 50);
   g_hero1 = new Hero(*texture, position1);
   CollisionItem *collision_item = new CollisionItem;
   collision_item->shape.type = CollisionShapeType::Sphere;
   collision_item->shape.sphere = { frame_size / 2, position1.x, position1.y };
   collision_item->material_type = 5;// todo prepare material list e.g. water, wall, pickable objects etc.
   m_world.add(collision_item);
   // todo game object (keeps graphic data and shape data reference)
}

void Game::processEvents() {
   sf::Event event;
   while (m_window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
      {
         m_window.close();
         break;
      }
      }
   }
}

void Game::update(sf::Time elapsed_time) {
   //update

   // handle players move
   //CollisionInfo collision;
   //// two approaches: update position and check collision or check line from old to new position
   //if (m_world.checkSingleCollision(player1.collision_item, collision))
   //{
   //   if (collision.item2->material_type == 1)  // water
   //   {
   //      // block
   //   }
   //   else if (collision.item2->material_type == 3)// wall
   //   {
   //      // slide
   //   }
   //}

   // add new barrel
   //if (...)
   //{
   //   // todo graphic data
   //   CollisionItem collision_item{};
   //   collision_item.shape.type = CollisionShapeType::Sphere;
   //   collision_item.shape.sphere = { radius, x, y };
   //   collision_item.material_type = 5;// todo prepare material list e.g. water, wall, pickable objects etc.
   //   m_world.add(collision_item);
   //   // todo game object (keeps graphic data and shape data reference)
   //}
}

void Game::render() {
   m_window.clear();
   //draw something
   m_window.draw(g_hero1->getSprite());
   m_window.display();
}

void Game::reportError(const std::string& msg)
{
   assert(false);
}
