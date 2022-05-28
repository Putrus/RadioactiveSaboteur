#include "../include/Game.h"
#include "../include/Hero.h"

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
   loadResources();
   m_hero1.reset(new Hero(m_texture_manager.get(TextureID::HERO1), sf::Vector2f(10.f, 10.f)));
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

void Game::newGame()
{
   //sf::Vector2f position1(50, 50);
   //g_hero1 = new Hero(*texture, position1);
   //CollisionItem *collision_item = new CollisionItem;
   //collision_item->shape.type = CollisionShapeType::Sphere;
   //collision_item->shape.sphere = { frame_size / 2, position1.x, position1.y };
   //collision_item->material_type = 5;// todo prepare material list e.g. water, wall, pickable objects etc.
   //m_world.add(collision_item);
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
      //to do change it to sf::Keyboard
      case sf::Event::KeyReleased:
      {
         if (event.key.code == sf::Keyboard::W && m_hero1->getSpeed().y < 0
            || event.key.code == sf::Keyboard::S && m_hero1->getSpeed().y > 0
            || event.key.code == sf::Keyboard::A && m_hero1->getSpeed().x < 0
            || event.key.code == sf::Keyboard::D && m_hero1->getSpeed().x > 0) {
            m_hero1->setAction(Action::IDLE);
         }
         break;
      }
      case sf::Event::KeyPressed:
      {
         if (m_hero1->getSpeed().y <= 0 && event.key.code == sf::Keyboard::W) {
            if (m_hero1->getSpeed().x < 0)
               m_hero1->setAction(Action::UPLEFT);
            else if (m_hero1->getSpeed().x > 0)
               m_hero1->setAction(Action::UPRIGHT);
            else
               m_hero1->setAction(Action::UP);
            break;
         }
         if (m_hero1->getSpeed().y >= 0 && event.key.code == sf::Keyboard::S) {
            if (m_hero1->getSpeed().x < 0)
               m_hero1->setAction(Action::DOWNLEFT);
            else if (m_hero1->getSpeed().x > 0)
               m_hero1->setAction(Action::DOWNRIGHT);
            else
               m_hero1->setAction(Action::DOWN);
            break;
         }
         if (m_hero1->getSpeed().x <= 0 && event.key.code == sf::Keyboard::A) {
            if (m_hero1->getSpeed().y < 0)
               m_hero1->setAction(Action::UPLEFT);
            else if (m_hero1->getSpeed().y > 0)
               m_hero1->setAction(Action::DOWNLEFT);
            else
               m_hero1->setAction(Action::LEFT);
            break;
         }
         if (m_hero1->getSpeed().x >= 0 && event.key.code == sf::Keyboard::D) {
            if (m_hero1->getSpeed().y < 0)
               m_hero1->setAction(Action::UPRIGHT);
            else if (m_hero1->getSpeed().y > 0)
               m_hero1->setAction(Action::DOWNRIGHT);
            else
               m_hero1->setAction(Action::RIGHT);
            break;
         }
         break;
      }
      }
   }
}

void Game::update(sf::Time elapsed_time) {
   //update

   // handle players move
   //CollisionInfo collision;
   //// two approaches: Action::UPdate position and check collision or check line from old to new position
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
   m_hero1->update(elapsed_time);
}

void Game::render() {
   m_window.clear();
   //draw something
   m_window.draw(m_hero1->getSprite());
   m_window.display();
}

void Game::reportError(const std::string& msg)
{
   assert(false);
}

void Game::loadResources() {
   std::string data_path = m_root_path + "data\\";
   m_texture_manager.load(HERO1, (data_path + "HERO1.png").c_str());
}