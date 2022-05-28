#include "../include/Game.h"

#include <SFML/Graphics.hpp>

Game::Game() : m_window(sf::VideoMode(400, 400), "Radioactive Saboteur"), m_time_per_frame(sf::seconds(1.f / 60.f)) {
}

void Game::run() {
   sf::Clock clock;
   sf::Time time_since_last_update = sf::Time::Zero;
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
   m_window.display();
}