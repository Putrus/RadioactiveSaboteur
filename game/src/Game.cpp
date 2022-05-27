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

         update(m_time_per_frame);
      }
      processEvents();
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
}

void Game::render() {
   m_window.clear();
   //draw something
   m_window.display();
}