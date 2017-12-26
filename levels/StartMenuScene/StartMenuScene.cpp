#include "StartMenuScene.h"
#include "BuilderScene.h"
#include "BuilderSceneECS.h"

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

#include "Circle.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "Callback.h"

#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"

#include "GraphicalSystem.h"
#include "PlayerInputSystem.h"

#include "AddToEntityMessage.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
{
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::init(Game& game) {
   // load fonts
   this->fonts().load("retro", "retro.ttf");
   
   // populate entities
   SceneObject* node = TextFactory::inst()->create_text_entity(
      "SFML TEST",
      this->fonts().get("retro"),
      sf::Vector2f(0, 0),
      36,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      false
   );
   this->scene_graph_->add(node);

   node = TextFactory::inst()->create_text_entity(
      "main menu",
      this->fonts().get("retro"),
      sf::Vector2f(0, 45),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      false
   );
   this->scene_graph_->add(node);

   node = TextFactory::inst()->create_text_entity(
      "(Press SPACE or ENTER)",
      this->fonts().get("retro"),
      sf::Vector2f(0, 60),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      false
   );
   this->scene_graph_->add(node);

   PlayerGamepad* pg = new PlayerGamepad("PlayerGamepad", this->fonts().get("retro"));
   this->gamepad(pg);

   pg->set(new ToggleDebugInfoCommand(this->scene_graph_, this->fonts().get("retro")), Key::O);
   pg->set(new SwitchSceneCommand(this, new BuilderScene()), Key::Space);
   pg->set(new SwitchSceneCommand(this, new BuilderSceneECS()), Key::Return);

   this->textures().load("tile_sign", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 0, 64, 64));
   
   GraphicalSystem* gs = dynamic_cast<GraphicalSystem*>(this->get_system("GraphicalSystem"));
   if (gs) {
      gs->camera()->resize(game.window().size());
      gs->camera()->reset_pan();
      gs->camera()->move(gs->camera()->get_center() - game.window().size());
   }

   this->add_system(new PlayerInputSystem());

   // test out the graphics system
   Handle e1 = this->create_entity();
   Handle e2 = this->create_entity();
   Handle e3 = this->create_entity();

   this->send_message_async<AddToEntityMessage>(e1, e2);
   this->send_message_async<AddToEntityMessage>(e1, e3);

   Entity* entity1 = this->get_entity(e1);
   Entity* entity2 = this->get_entity(e2);
   Entity* entity3 = this->get_entity(e3);

   entity1->id("Rectangle Entity");
   entity2->id("Left Circle Entity");
   entity3->id("Right Circle Entity");

   entity1->add<Rectangle>();
   entity2->add<Circle>();
   entity3->add<Circle>();

   Rectangle* shaft = entity1->get<Rectangle>();
   shaft->position(-100, -70);
   shaft->size(400, 150);
   shaft->color(sf::Color::Red);

   entity1->add<Callback>();
   entity1->get<Callback>()->mouse_in([&, entity1] () {
      Game::logger().msg(entity1->id(), Logger::INFO, "POOP");
   });

   Circle* right_ball = entity2->get<Circle>();
   right_ball->position(-200, 0);
   right_ball->radius(100);
   right_ball->color(sf::Color::Blue);

   Circle* left_ball = entity3->get<Circle>();
   left_ball->position(-200, -200);
   left_ball->radius(100);
   left_ball->color(sf::Color::Blue);

   game.add_player(1);

   game.get_player(1).name("Player1");
   game.get_player(1).bindings().set<MoveLeftIntent>(1, game.input_manager().get_device(1)->get("Left"));
   game.get_player(1).bindings().set<MoveRightIntent>(1, game.input_manager().get_device(1)->get("Right"));
}

void StartMenuScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering game start menu state.");
   this->camera_->reset_pan();
   this->camera_->move(this->camera_->get_center() - game.window().size());
}

void StartMenuScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::process(Game& game, ResizeInputEvent& e) {
   Scene::process(game, e);

   // re-adjust centering
   this->camera_->reset_pan();
   this->camera_->move(this->camera_->get_center() - game.window().size());
}
