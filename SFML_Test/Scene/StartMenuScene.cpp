#include "StartMenuScene.h"
#include "BuilderScene.h"

#include "UtilFactory.h"
#include "TextFactory.h"

#include "Game.h"
#include "Graphics.h"
#include "Layer.h"

#include "Graphic.h"
#include "GraphicsPart.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
{
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering game start menu state.");

   // create viewport(s)
   this->viewport_ = new Viewport(sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height()));
   this->viewport_->add("debug");

   // populate entities
   Entity* title = TextFactory::inst()->create_text_entity(
      "SFML TEST",
      "retro",
      this->viewport_->layer("main")->get_center(),
      36,
      TextFactory::ALIGN::CENTER
   );

   Entity* subtitle1 = TextFactory::inst()->create_text_entity(
      "main menu",
      "retro",
      this->viewport_->layer("main")->get_center() + sf::Vector2f(0, 45),
      12,
      TextFactory::ALIGN::CENTER
   );

   Entity* subtitle2 = TextFactory::inst()->create_text_entity(
      "(Press SPACE or ENTER)",
      "retro",
      this->viewport_->layer("main")->get_center() + sf::Vector2f(0, 60),
      12,
      TextFactory::ALIGN::CENTER
   );

   // add to update list
   this->entities_.push_back(title);
   this->entities_.push_back(subtitle1);
   this->entities_.push_back(subtitle2);

   // add to draw list
   this->viewport_->layer("main")->add(title);
   this->viewport_->layer("main")->add(subtitle1);
   this->viewport_->layer("main")->add(subtitle2);
}

void StartMenuScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::update(Game& game, Scene* scene, Entity* entity) {
   Scene::update(game, scene, entity);
}

void StartMenuScene::process(Game& game, CloseCommand& c) {
   game.unload_scene();
}

void StartMenuScene::process(Game& game, KeyPressCommand& c) {
   if (c.event.code == sf::Keyboard::Key::Space || c.event.code == sf::Keyboard::Key::Return) {
      game.switch_scene(new BuilderScene());
   } else if (c.event.code == sf::Keyboard::Key::O) {
      EntityList::const_iterator it;
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         GraphicsPart* d = dynamic_cast<GraphicsPart*>((*it)->get("debug"));
         if (d) {
            this->viewport_->layer("debug")->remove(d);
            (*it)->remove("debug");
            continue;
         }

         GraphicsPart* graphics_part = dynamic_cast<GraphicsPart*>((*it)->get("graphics"));
         sf::FloatRect bounds(graphics_part->get(0)->get_global_bounds());
         d = UtilFactory::inst()->create_debug_graphics(bounds);

         (*it)->add(d);
         this->viewport_->layer("debug")->add(d);
      }
   }
}

void StartMenuScene::process(Game& game, WindowResizeCommand& c) {
   sf::Vector2f offset;
   sf::Vector2f new_size(c.width, c.height);
   sf::Vector2f new_center(c.width / 2.f, c.height / 2.f);

   this->viewport_->resize(new_size);
   this->viewport_->recenter(new_center);

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
