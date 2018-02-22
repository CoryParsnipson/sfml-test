#include "StartMenuScene.h"

#include "Game.h"
#include "Entity.h"

#include "StartMenuInputSystem.h"

#include "Text.h"
#include "Rectangle.h"
#include "Callback.h"
#include "PlayerProfile.h"

#include "MouseXIntent.h"
#include "MouseYIntent.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
{
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::init(Game& game) {
   // load fonts
   this->fonts().load("retro", "retro.ttf");
   
   // setup player's input mapping
   game.add_player(1);

   game.get_player(1).bindings().set<MouseXIntent>(0, game.input_manager().get_device(0)->get("PositionX"));
   game.get_player(1).bindings().set<MouseYIntent>(0, game.input_manager().get_device(0)->get("PositionY"));

   // create title text entities
   Entity* title = this->create_entity();
   title->id("Title Entity");

   title->add<Text>("TitleText", "SFML TEST", this->fonts().get("retro"), 36);
   title->get<Text>()->color(sf::Color::White);

   // center text
   title->get<Text>()->origin(
      title->get<Text>()->local_bounds().left + title->get<Text>()->local_bounds().width / 2.f,
      title->get<Text>()->local_bounds().top + title->get<Text>()->local_bounds().height/ 2.f
   );

   title->get<Text>()->position(0, 0); // must position after changing origin

   Entity* subtitle = this->create_entity();
   subtitle->id("SubtitleEntity");

   subtitle->add<Text>("SubtitleText", "main menu", this->fonts().get("retro"), 12);
   subtitle->get<Text>()->color(sf::Color::White);

   subtitle->get<Text>()->origin(
      subtitle->get<Text>()->local_bounds().left + subtitle->get<Text>()->local_bounds().width / 2.f,
      subtitle->get<Text>()->local_bounds().top + subtitle->get<Text>()->local_bounds().height/ 2.f
   );

   subtitle->get<Text>()->position(0, 45);

   Entity* subtitle2 = this->create_entity();
   subtitle2->id("Subtitle2Entity");

   subtitle2->add<Text>("Subtitle2Text", "(Press SPACE or ENTER)", this->fonts().get("retro"), 12);
   subtitle2->get<Text>()->color(sf::Color::White);

   subtitle2->get<Text>()->origin(
      subtitle2->get<Text>()->local_bounds().left + subtitle2->get<Text>()->local_bounds().width / 2.f,
      subtitle2->get<Text>()->local_bounds().top + subtitle2->get<Text>()->local_bounds().height/ 2.f
   );

   subtitle2->get<Text>()->position(0, 60);

   // center text entities in the middle of the screen
   Entity* text_layer = this->create_entity();
   text_layer->add<PlayerProfile>("RootPlayerProfile", 1);
   text_layer->add<Callback>("RootCallback");
   
   // TODO: create a better way to get Systems
   GraphicalSystem* gs = dynamic_cast<GraphicalSystem*>(this->get_system("GraphicalSystem"));

   text_layer->get<Callback>()->camera_resize([text_layer, gs] () {
      text_layer->get<Space>()->position(gs->camera()->center());
   });

   this->send_message_async<AddToEntityMessage>(text_layer->handle(), title->handle());
   this->send_message_async<AddToEntityMessage>(text_layer->handle(), subtitle->handle());
   this->send_message_async<AddToEntityMessage>(text_layer->handle(), subtitle2->handle());

   // create mouse cursor
   Entity* mouse_cursor = this->create_entity();
   mouse_cursor->id("MouseCursorEntity");

   this->mouse_cursor_ = mouse_cursor->handle();

   mouse_cursor->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   mouse_cursor->add<Rectangle>("MouseCursorRectangle", 0, 0, 6, 6);
   mouse_cursor->get<Rectangle>()->color(sf::Color::Red);

   mouse_cursor->add<Text>("MouseCursorText", "0, 0", this->fonts().get("retro"), 12);
   mouse_cursor->get<Text>()->position(0, 10);
   mouse_cursor->get<Text>()->color(sf::Color::White);

   mouse_cursor->add<Callback>("MouseCursorCallback");
   mouse_cursor->get<Callback>()->mouse_move([mouse_cursor, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      // move mouse cursor
      mouse_cursor->get<Rectangle>()->position(new_pos - sf::Vector2f(3, 3));

      // move and update mouse cursor text
      mouse_cursor->get<Text>()->position(new_pos + sf::Vector2f(0, 10));
      mouse_cursor->get<Text>()->string(
         std::to_string(static_cast<int>(new_pos.x)) +
         ", " +
         std::to_string(static_cast<int>(new_pos.y))
      );
   });

   // add input system
   this->add_system(new StartMenuInputSystem());
}

void StartMenuScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering game start menu state.");
}

void StartMenuScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::process(Game& game, MouseEnteredInputEvent& e) {
   Entity* cursor = this->get_entity(this->mouse_cursor_);
   if (cursor) {
      cursor->get<Space>()->visible(true);
   }
}

void StartMenuScene::process(Game& game, MouseLeftInputEvent& e) {
   Entity* cursor = this->get_entity(this->mouse_cursor_);
   if (cursor) {
      cursor->get<Space>()->visible(false);
   }
}
