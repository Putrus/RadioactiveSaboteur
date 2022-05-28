#include "../include/Game.h"
#include "../include/FixedObject.h"
#include "../include/Hero.h"
#include "../include/Math.h"

#include <SFML/Graphics.hpp>
#include <cassert>

Game::Game(const std::string& root_path) :
   m_root_path(root_path),
   m_window(sf::VideoMode(1024, 800), "Radioactive Saboteur"), m_time_per_frame(sf::seconds(1.f / 60.f))
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
   auto create_water_collision_item = [this](const sf::IntRect &rect)
   {
      CollisionItem* collision_item = new CollisionItem;
      collision_item->shape.type = CollisionShapeType::AxisAlignedBoundingBox;
      collision_item->shape.aabb.left = rect.left;
      collision_item->shape.aabb.right = rect.left + rect.width;
      collision_item->shape.aabb.top = rect.top;
      collision_item->shape.aabb.bottom = rect.top + rect.height;
      collision_item->material_type = MT_Water;
      m_world.add(collision_item);
   };

   srand(time(0));
   sf::Texture &bkg = m_texture_manager.get(TEX_BACKGROUND);

   for (int y = 0; y < 36; ++y)
   {
      for (int x = 0; x < 64; ++x)
      {

         int type = rand() % 8;

         sf::IntRect source_rect(30 * (type % 7), 30 * (type / 7), 30, 30);
         sf::Vector2f target_position(x * 30, y * 30);
         sf::IntRect target_rect(target_position.x, target_position.y, 30, 30);
         if (type < 900)
            m_background.addSprite(bkg, source_rect, target_position);
         else
         {
            m_background.addSprite(bkg, source_rect, target_position);
            create_water_collision_item(target_rect);
         }
      }
   }

   sf::Vector2f pos(10, 10);
   CollisionItem *collision_item = new CollisionItem;
   collision_item->shape.type = CollisionShapeType::Sphere;
   collision_item->shape.sphere = { pos.x, pos.y, frame_size / 2 };
   collision_item->material_type = MT_Hero;
   bomba.reset(new Hero(m_texture_manager.get(TEX_HERO1), *collision_item, sf::Vector2f(10.f, 10.f)));
   //kurvinox.reset(new Hero(m_texture_manager.get(TEX_HERO2), *collision_item, sf::Vector2f(500.f, 210.f)));
   g_renderables.push_back(&bomba->getSprite());
   //g_renderables.push_back(&kurwinox->getSprite());

   pos = sf::Vector2f(250, 250);
   FixedObject* object = new FixedObject(m_texture_manager.get(TEX_TREE1), pos, sf::Vector2f(31, 115));
   collision_item->shape.sphere = { pos.x, pos.y, 10 };
   collision_item->material_type = MT_Tree;
   m_world.add(collision_item);
   g_renderables.push_back(&object->getSprite());
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
      }
      processPlayerEvents(*bomba, event, 0);
      //processPlayerEvents(*kurwinox, event, 1);
   }
}

void Game::update(sf::Time elapsed_time) {
   // handle players move
   // two approaches:
   // - update position and check collision afterwards
   // - check line from old to new position
   if (math::length(bomba->getSpeed()) > 0)
   {
      sf::Vector2f pos = bomba->getPosition();
      sf::Vector2f target = pos + bomba->getSpeed();
      CollisionInfo collision;
      if (m_world.checkSingleCollision(pos, target, collision))
      {
         if (collision.item1->material_type == MT_Barrel)
         {
            assert(false);//not implemented
         }
         // blocking objects
         else
         {
            float collision_distance = math::distance(pos, collision.point);
            if (collision_distance > bomba->getRadius())
            {
               // adjust hero position
               sf::Vector2f offset = target - pos;
               math::set_length(offset, collision_distance - 1);
               pos += offset;
               bomba->setPosition(target);
            }
            // else don't move
         }

         //if (collision.item2->material_type == 1)  // water
         //{
         //   // block
         //}
         //else if (collision.item2->material_type == 3)// wall
         //{
         //   // slide
         //}
      }
      else
         bomba->setPosition(target);
   }
   bomba->update(elapsed_time);
   m_background.update(elapsed_time);
   //kurwinox->update(elapsed_time);

   // add new barrel
   //if (...)
   //{
   //   // todo graphic data
   //   CollisionItem collision_item{};
   //   collision_item.shape.type = CollisionShapeType::Sphere;
   //   collision_item.shape.sphere = { x, y, radius };
   //   collision_item.material_type = 5;// todo prepare material list e.g. water, wall, pickable objects etc.
   //   m_world.add(collision_item);
   //   // todo game object (keeps graphic data and shape data reference)
   //}

}

void Game::render() {
   m_window.clear();
   
   m_background.draw(m_window);

   m_window.draw(bomba->getSprite());

   for (const sf::Drawable* drawable : g_renderables)
   {
      m_window.draw(*drawable);
   }
   m_window.display();
}

void Game::reportError(const std::string& msg)
{
   assert(false);
}

void Game::loadResources() {
   std::string data_path = m_root_path + "data\\";
   m_texture_manager.load(TEX_HERO1, (data_path + "HERO1.png").c_str());
   m_texture_manager.load(TEX_HERO2, (data_path + "HERO2.png").c_str());
   m_texture_manager.load(TEX_TREE1, (data_path + "tree_1.png").c_str());
   m_texture_manager.load(TEX_BACKGROUND, (data_path + "BACKGROUND.png").c_str());
}

void Game::processPlayerEvents(Hero& hero, const sf::Event& event, bool player) {
   std::vector<sf::Keyboard::Key> controls;
   if (player) {
      controls = { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right };
   }
   else {
      controls = { sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D };
   }
   switch (event.type) {
   case sf::Event::KeyReleased:
   {
      //player1
      if (event.key.code == controls[0] && hero.getSpeed().y < 0
         || event.key.code == controls[1] && hero.getSpeed().y > 0
         || event.key.code == controls[2] && hero.getSpeed().x < 0
         || event.key.code == controls[3] && hero.getSpeed().x > 0) {
         hero.setAction(Action::IDLE);
      }
      break;
   }
   case sf::Event::KeyPressed:
   {
      //player1
      if (hero.getSpeed().y <= 0 && event.key.code == controls[0]) {
         if (hero.getSpeed().x < 0)
            hero.setAction(Action::UPLEFT);
         else if (hero.getSpeed().x > 0)
            hero.setAction(Action::UPRIGHT);
         else
            hero.setAction(Action::UP);
         break;
      }
      if (hero.getSpeed().y >= 0 && event.key.code == controls[1]) {
         if (hero.getSpeed().x < 0)
            hero.setAction(Action::DOWNLEFT);
         else if (hero.getSpeed().x > 0)
            hero.setAction(Action::DOWNRIGHT);
         else
            hero.setAction(Action::DOWN);
         break;
      }
      if (hero.getSpeed().x <= 0 && event.key.code == controls[2]) {
         if (hero.getSpeed().y < 0)
            hero.setAction(Action::UPLEFT);
         else if (hero.getSpeed().y > 0)
            hero.setAction(Action::DOWNLEFT);
         else
            hero.setAction(Action::LEFT);
         break;
      }
      if (hero.getSpeed().x >= 0 && event.key.code == controls[3]) {
         if (hero.getSpeed().y < 0)
            hero.setAction(Action::UPRIGHT);
         else if (hero.getSpeed().y > 0)
            hero.setAction(Action::DOWNRIGHT);
         else
            hero.setAction(Action::RIGHT);
         break;
      }
      break;
   }
   }
}