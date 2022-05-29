#include "../include/Game.h"
#include "../include/FixedObject.h"
#include "../include/Hero.h"
#include "../include/Math.h"

#include <SFML/Graphics.hpp>
#include <cassert>

/*
Must have:
x respawn barrel only if it was dropped first time (ignore moved barrels)
x second player
x second player: button for dropping item (first player uses space)
- calculate statistics & win/lose

Optional:
- menu
x slow down in water
- comtaminate area - improve
- rendering: sort by y value
*/

struct HardcodedSettings
{
   float hero_aabb_size = 32;
   //sf::Vector2f barrel_origin(30, 30);
   float barrel_origin_x = 30;
   float barrel_origin_y = 30;
   int comtaminate_cycles = 3;
   sf::Color comtaminate_color = sf::Color(150, 255, 150);
} hardcoded;

struct ConstantPlayerSettings
{
   sf::Vector2f start_position;
   sf::Vector2f barrel_spawn_location;
} player_settings[2];

struct PlayerStats
{
   int total_wins = 0;
} player_stats[2];


Game::Game(const std::string& root_path) :
   m_root_path(root_path),
   m_window(sf::VideoMode(1024, 800), "Radioactive Saboteur", sf::Style::Fullscreen), m_time_per_frame(sf::seconds(1.f / 60.f)),
   m_print_debug_info(false),
   m_bomba_collider(nullptr),
   m_kurvinox_collider(nullptr)
{
   if (m_root_path.size() > 0)
   {
      if (m_root_path.back() != '/' && m_root_path.back() != '\\')
         m_root_path += '\\';
   }

   player_settings[0].start_position = sf::Vector2f(20, 20);
   player_settings[1].start_position = sf::Vector2f(780, 20);
   player_settings[0].barrel_spawn_location = sf::Vector2f(50, 300);
   player_settings[1].barrel_spawn_location = sf::Vector2f(750, 300);
}

void Game::run() {
   sf::Clock clock;
   sf::Time time_since_last_update = sf::Time::Zero;
   // todo connect with menu handler at the end!
   loadResources();
   m_menu.reset(new Menu(m_texture_manager.get(TEX_MENU)));
   //m_menu->setVisible(false);
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
         bool is_water_one_above = m_background.isWater(x, y - 1);
         int type = m_background.isWater(x-1, y) || is_water_one_above ? rand() % 12 : rand() % 8;
         sf::IntRect source_rect(
            background_field_size * (type % 7), background_field_size * (type / 7),
            background_field_size, background_field_size);
         if (source_rect.top != 0 && is_water_one_above) {
            source_rect.top += background_field_size;
         }
         sf::Vector2f target_position(x * background_field_size, y * background_field_size);
         sf::IntRect target_rect(target_position.x, target_position.y, background_field_size, background_field_size);
         if (type < 7)
            m_background.addSprite(bkg, source_rect, target_position);
         else
         {
            m_background.addSprite(bkg, source_rect, target_position);
            create_water_collision_item(target_rect);
         }
      }
   }

   auto add_player = [this](int player_index, int texture_id, 
      std::unique_ptr<Hero> &target_hero, CollisionItem** collision_item_target)
   {
      //sf::Vector2f pos(10, 10);
      //CollisionItem *collision_item = new CollisionItem;
      //collision_item->shape.type = CollisionShapeType::Sphere;
      //collision_item->shape.sphere = { pos.x, pos.y, frame_size / 2 };
      //collision_item->material_type = MT_Hero;
      //target_hero.reset(new Hero(m_texture_manager.get(TEX_HERO1), *collision_item, sf::Vector2f(10.f, 10.f)));
      ////kurvinox.reset(new Hero(m_texture_manager.get(TEX_HERO2), *collision_item, sf::Vector2f(500.f, 210.f)));
      //m_renderables.push_back(&target_hero->getSprite());
      ////m_renderables.push_back(&kurwinox->getSprite());
      sf::Vector2f pos = player_settings[player_index].start_position;
      auto *collision_item = new CollisionItem;
      collision_item->shape.type = CollisionShapeType::AxisAlignedBoundingBox;
      collision_item->shape.aabb = make_aabb(
         pos.x - hardcoded.hero_aabb_size / 2, pos.y - hardcoded.hero_aabb_size / 2,
         hardcoded.hero_aabb_size, hardcoded.hero_aabb_size);
      collision_item->material_type = MT_Hero;
      target_hero.reset(new Hero(m_texture_manager.get(TextureID(texture_id)), *collision_item, pos));
      m_renderables.push_back(&target_hero->getSprite());
      *collision_item_target = collision_item;
   };
   // bomba
   add_player(0, TEX_HERO1, bomba, &m_bomba_collider);
   add_player(1, TEX_HERO2, kurwinox, &m_kurvinox_collider);

   // tree
   sf::Vector2f pos = sf::Vector2f(250, 250);
   CollisionItem* collision_item = new CollisionItem;
   FixedObject* object = new FixedObject(m_texture_manager.get(TEX_TREE1),
      *collision_item, pos, sf::Vector2f(31, 115));
   collision_item->shape.type = CollisionShapeType::AxisAlignedBoundingBox;
   collision_item->shape.aabb = make_aabb(pos.x, pos.y, 32, 32);
   collision_item->material_type = MT_Hero;
   //collision_item->shape.sphere = { pos.x, pos.y, 10 };
   collision_item->material_type = MT_Tree;
   m_world.add(collision_item);
   m_renderables.push_back(&object->getSprite());

   addNewBarrel(player_settings[0].barrel_spawn_location);
   addNewBarrel(player_settings[1].barrel_spawn_location);
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
      case sf::Event::KeyReleased:
      {
         if (event.key.code == sf::Keyboard::F1)
            m_print_debug_info = !m_print_debug_info;
         break;
      }
      case sf::Event::MouseButtonPressed:
      {
         if (event.key.code == sf::Mouse::Left) {
         }
      }
      //to do change it to sf::Keyboard
      }
      processPlayerEvents(*bomba, event, 0);
      processPlayerEvents(*kurwinox, event, 1);
   }
}

void Game::updateSinglePlayer(Hero& player, CollisionItem& collider, sf::Time elapsed_time)
{
   // handle players move
// two approaches:
// - update position and check collision afterwards
// - check line from old to new position
   if (math::length(player.getSpeed()) > 0)
   {
      sf::Vector2f pos = player.getPosition();
      sf::Vector2f target = pos + player.getSpeed();
      sf::Vector2f offset = target - pos;
      AABB next_aabb = collider.shape.aabb;
      offset_aabb(next_aabb, offset.x, offset.y);
      CollisionInfo collisions[4];
      //if (m_world.checkSingleCollision(pos, target, collision))//sphere
      // todo it should be sorted by distance to collision point
      int col_count = m_world.checkManyCollisions(next_aabb, collisions, 4);
      if (col_count > 0)
      {
         for (int idx = 0; idx < col_count; ++idx)
         {
            if (collisions[idx].item1->material_type == MT_Barrel)
            {
               FixedObject* barrel = static_cast<FixedObject*>(collisions[idx].item1->custom_data);
               assert(barrel);
               if (math::distance(player.getPosition(), barrel->getPosition()) < 10)
               {
                  if (player.isBackpackEmpty())
                  {
                     player.barrelToBackpack(barrel);
                     m_world.remove(&barrel->getCollisionItem());
                  }
               }
               else
                  player.setPosition(target);
            }
            else if (collisions[idx].item1->material_type == MaterialType::MT_Water)
            {
               player.setWater(true);
               player.setPosition(target);
            }
            // blocking objects
            else
            {
               float collision_distance = math::distance(pos, collisions[idx].point);
               if (collision_distance > player.getRadius())
               {
                  // adjust hero position
                  sf::Vector2f offset = target - pos;
                  math::set_length(offset, collision_distance - 1);
                  pos += offset;
                  player.setPosition(target);
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
      }
      else
      {
         player.setWater(false);
         player.setPosition(target);
      }
   }
   player.update(elapsed_time);
}

void Game::update(sf::Time elapsed_time) {
   updateSinglePlayer(*bomba, *m_bomba_collider, elapsed_time);
   updateSinglePlayer(*kurwinox, *m_kurvinox_collider, elapsed_time);
   
   m_menu->update(sf::Mouse::getPosition(m_window));
   m_background.update(elapsed_time);
}

void Game::render() {
   m_window.clear();
   
   if (!m_menu->isVisible()) {

      m_background.draw(m_window);

      // already in m_renderables
      //m_window.draw(bomba->getSprite());
      //m_window.draw(kurwinox->getSprite());

      for (const sf::Drawable* drawable : m_renderables)
      {
         m_window.draw(*drawable);
      }

      if (m_print_debug_info)
      {
         std::string debug_text = "Collider: L: "
            + std::to_string(m_bomba_collider->shape.aabb.left) + " T: "
            + std::to_string(m_bomba_collider->shape.aabb.top) + " R: "
            + std::to_string(m_bomba_collider->shape.aabb.right) + " B: "
            + std::to_string(m_bomba_collider->shape.aabb.bottom) + "\n"
            + "Sprite pos: " + std::to_string(bomba->getPosition().x) + " " + std::to_string(bomba->getPosition().y);
         sf::Text text;
         text.setFont(m_debug_font);
         text.setString(debug_text);
         text.setCharacterSize(20); // in pixels, not points!
         text.setFillColor(sf::Color::Yellow);
         m_window.draw(text);
      }
   }
   else {
      m_menu->draw(m_window);
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
   m_texture_manager.load(TEX_BARREL, (data_path + "BARREL1.png").c_str());
   m_texture_manager.load(TEX_MENU, (data_path + "MENU.png").c_str());

   //std::string font_file = "arial.ttf";
   std::string font_file = "consola.ttf";
   m_debug_font.loadFromFile(data_path + font_file);
   m_font_manager.load(FONT_PIXEL, (data_path + "pixel.ttf").c_str());
}

// 0: bomba
// 1: kurvinox
void Game::processPlayerEvents(Hero& hero, const sf::Event& event, int player) {
   std::vector<sf::Keyboard::Key> controls;
   if (player) {
      controls = { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::RControl };
   }
   else {
      controls = { sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::Space };
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
         //debug purpose only
         //m_background.contaminateArea(10, 10, hardcoded.comtaminate_color);
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
      if (event.key.code == controls[4])
      {
         // null if space pressed without holding any object
         FixedObject* barrel = hero.dropBarrel();
         if (barrel)
         {
            // in idle we don't have info about orientation so we need the previous action
            Action action = hero.getAction();
            // todo if idle then drop in the direction of enemy area
            if (action == Action::IDLE)
               action = hero.getPreviousAction();

            static const Action opposite_directions[] =
            {
               IDLE,    //IDLE = 0,
               UP,      //DOWN,
               LEFT,    //RIGHT,
               RIGHT,   //LEFT,
               DOWN,    //UP,
               DOWNRIGHT,  //UPLEFT,
               DOWNLEFT,   //UPRIGHT,
               UPRIGHT,    //DOWNLEFT,
               UPLEFT,     //DOWNRIGHT
            };
            sf::Vector2f new_position;
            Action reversed_action = opposite_directions[action];
            float off_x = 0, off_y = 0;
            if (reversed_action == LEFT || reversed_action == DOWNLEFT || reversed_action == UPLEFT)
               off_x = -hardcoded.hero_aabb_size;
            else if (reversed_action == RIGHT || reversed_action == DOWNRIGHT || reversed_action == UPRIGHT)
               off_x = hardcoded.hero_aabb_size;
            if (reversed_action == UP || reversed_action == UPLEFT || reversed_action == UPRIGHT)
               off_y = -hardcoded.hero_aabb_size;
            else if (reversed_action == DOWN || reversed_action == DOWNLEFT || reversed_action == DOWNRIGHT)
               off_y = hardcoded.hero_aabb_size;

            new_position = barrel->getPosition() + sf::Vector2f(off_x, off_y);
            barrel->setPosition(new_position);
            barrel->getCollisionItem().shape.aabb = make_aabb(
                  barrel->getPosition().x - hardcoded.barrel_origin_x / 2,
                  barrel->getPosition().y - hardcoded.barrel_origin_y / 2,
                  hardcoded.barrel_origin_x, hardcoded.barrel_origin_y);
            m_world.add(&barrel->getCollisionItem());
            if (!barrel->isTouched())
               addNewBarrel(player_settings[player].barrel_spawn_location);
            // radioactivity 3..2..1
            std::pair<int, int> origin_field = m_background.getField(barrel->getPosition());
            m_background.addContaminateSource(origin_field.first, origin_field.second,
               hardcoded.comtaminate_color, hardcoded.comtaminate_cycles);
            barrel->touch();
         }
      }
      break;
   }
   }
}

void Game::addNewBarrel(sf::Vector2f position)
{
   CollisionItem* collision_item = new CollisionItem;
   collision_item->shape.type = CollisionShapeType::AxisAlignedBoundingBox;
   collision_item->shape.aabb = make_aabb(
      position.x - hardcoded.barrel_origin_x / 2,
      position.y - hardcoded.barrel_origin_y / 2,
      hardcoded.barrel_origin_x, hardcoded.barrel_origin_y);
   collision_item->material_type = MT_Barrel;

   sf::Texture& texture = m_texture_manager.get(TEX_BARREL);
   FixedObject* object = new FixedObject(texture, *collision_item, position, sf::Vector2f(15, 40));
   m_renderables.push_back(&object->getSprite());

   collision_item->custom_data = object;
   m_world.add(collision_item);
}
