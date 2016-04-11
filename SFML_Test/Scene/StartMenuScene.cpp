#include "StartMenuScene.h"
#include "BuilderScene.h"
#include "TestUIScene.h"

#include "UtilFactory.h"
#include "TextFactory.h"

#include "Game.h"

#include "Graphic.h"
#include "GraphicsPart.h"
#include "DebugPart.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
, show_debug_info_(false)
{
   // populate entities
   this->entities_.push_back(TextFactory::inst()->create_text_entity(
      "SFML TEST",
      "retro",
      this->camera_->get_center(),
      36,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      this->show_debug_info_
   ));

   this->entities_.push_back(TextFactory::inst()->create_text_entity(
      "main menu",
      "retro",
      this->camera_->get_center() + sf::Vector2f(0, 45),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      this->show_debug_info_
   ));

   this->entities_.push_back(TextFactory::inst()->create_text_entity(
      "(Press SPACE or ENTER)",
      "retro",
      this->camera_->get_center() + sf::Vector2f(0, 60),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      0,
      this->show_debug_info_
   ));
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::draw(RenderSurface& surface, sf::RenderStates render_states) {
   TextureManager::inst()->create_texture("1", "flatmap_test_texture.png", sf::IntRect(0, 0, 40, 40));

   Graphic2* g2 = new SpriteGraphic(TextureManager::inst()->get_texture("1"));
   g2->set_size(40, 40);
   g2->set_position(300, 50);

   Graphic2* g = new SpriteGraphic(TextureManager::inst()->get_texture("1"));
   g->set_size(40, 40);
   g->set_color(sf::Color::Red);
   g->set_position(100, 100);

   g->add(g2);
   g->draw(surface, render_states);

   Scene::draw(surface, render_states);
}

void StartMenuScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering game start menu state.");
}

void StartMenuScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::update(Game& game, Scene* scene, Entity* entity) {
   Scene::update(game, scene, entity);
}

void StartMenuScene::process(Game& game, CloseCommand& c) {
   Scene::process(game, c);
}

void StartMenuScene::process(Game& game, KeyPressCommand& c) {
   if (c.event.code == sf::Keyboard::Key::Space || c.event.code == sf::Keyboard::Key::Return) {
      game.switch_scene(new BuilderScene());
   } else if (c.event.code == sf::Keyboard::Key::Escape) {
      // go to super special awesome test scene
      game.switch_scene(new TestUIScene());
   } else if (c.event.code == sf::Keyboard::Key::O) {
      this->toggle_debug_info();
   }
}

void StartMenuScene::process(Game& game, WindowResizeCommand& c) {
   Scene::process(game, c);

   sf::Vector2f offset;
   sf::Vector2f new_size(c.width, c.height);
   sf::Vector2f new_center(c.width / 2.f, c.height / 2.f);

   // readjust all entities
   EntityList::const_iterator it;
   for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
      (*it)->set_position(new_center + offset);

      GraphicsPart* graphics = dynamic_cast<GraphicsPart*>((*it)->get("graphics"));
      offset.y += (2 + graphics->get(0)->get_size().y);
   }
}

void StartMenuScene::process(Game& game, MouseMoveCommand& c) {}
void StartMenuScene::process(Game& game, MouseButtonCommand& c) {}
void StartMenuScene::process(Game& game, MouseWheelCommand& c) {}

void StartMenuScene::toggle_debug_info() {
   EntityList::const_iterator it;

   this->show_debug_info_ = !this->show_debug_info_;

   if (this->show_debug_info_) {
      // turned debug info on, add debug components to all entities
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         (*it)->add(new DebugPart());
      }
   } else {
      // turned debug info off, remove debug components from all entities
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         (*it)->remove("debug");
      }
   }
}
