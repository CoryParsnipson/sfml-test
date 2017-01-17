#include "StartMenuScene.h"
#include "BuilderScene.h"
#include "TestUIScene.h"

#include "UtilFactory.h"
#include "TextFactory.h"

#include "Game.h"

#include "Draw.h"
#include "GraphicsPart.h"
#include "DebugPart.h"
#include "SceneObject.h"

#include "SwitchSceneCommand.h"
#include "ToggleDebugInfoCommand.h"

#include "PlayerGamepad.h"

#include "FileChannel.h"
#include "JSONSerializer.h"
#include "Entity.h"
#include "PhysicsPart.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
{
   Channel* c = new FileChannel("test.txt");

   Serializer* s = new JSONSerializer(3);
   Entity* e = new Entity();
   e->add(new PhysicsPart("let's talk about christmas trees", sf::FloatRect(100, 77, 23, 23)));
   e->add(new PhysicsPart("physics2", sf::FloatRect(10, 20, 1, 1)));

   std::string str = e->serialize(*s);
   std::cout << "Entity serialized: " << str << std::endl;

   c->seek(0);
   bool res = c->send(str);

   if (!res) {
      std::cout << "ERROR" << std::endl;
   }

   delete c;
   delete s;
   delete e;




   // load fonts
   this->fonts_.load("retro", "retro.ttf");
   
   // populate entities
   SceneObject* node = TextFactory::inst()->create_text_entity(
      "SFML TEST",
      this->fonts_.get("retro"),
      sf::Vector2f(0, 0),
      36,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      false
   );
   this->scene_graph_->add(node);

   node = TextFactory::inst()->create_text_entity(
      "main menu",
      this->fonts_.get("retro"),
      sf::Vector2f(0, 45),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      false
   );
   this->scene_graph_->add(node);

   node = TextFactory::inst()->create_text_entity(
      "(Press SPACE or ENTER)",
      this->fonts_.get("retro"),
      sf::Vector2f(0, 60),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      false
   );
   this->scene_graph_->add(node);

   PlayerGamepad* pg = new PlayerGamepad("PlayerGamepad", this->fonts_.get("retro"));
   this->gamepad(pg);

   pg->set(new ToggleDebugInfoCommand(this->scene_graph_, this->fonts_.get("retro")), Key::O);
   pg->set(new SwitchSceneCommand(this, new BuilderScene()), Key::Space);
   pg->set(new SwitchSceneCommand(this, new BuilderScene()), Key::Return);
   pg->set(new SwitchSceneCommand(this, new TestUIScene()), Key::Escape);
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering game start menu state.");
   this->camera_->reset_pan();
   this->camera_->move(this->camera_->get_center() - game.window().size());
}

void StartMenuScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::process(Game& game, ResizeInputEvent& e) {
   Scene::process(game, e);

   // re-adjust centering
   this->camera_->reset_pan();
   this->camera_->move(this->camera_->get_center() - game.window().size());
}
