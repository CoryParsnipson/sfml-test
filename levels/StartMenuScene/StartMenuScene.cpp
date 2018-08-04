#include "StartMenuScene.h"

#include "BuilderScene.h"

#include "Game.h"
#include "Entity.h"
#include "Color.h"
#include "Animation.h"

#include "Text.h"
#include "Sprite.h"
#include "Rectangle.h"
#include "Callback.h"
#include "PlayerProfile.h"

#include "MoveUpIntent.h"
#include "MoveDownIntent.h"
#include "SelectMenuItemIntent.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
, menu_select_(0)
{
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::init(Game& game) {
   // load fonts
   this->fonts().load("retro", "retro.ttf");

   // load textures
   this->textures().load("cursor", "cursor.png");
   
   // setup player's input mapping
   game.add_player(1);

   game.get_player(1).bindings().set<MoveUpIntent>(1, game.input_manager().get_device(1)->get("Up"));
   game.get_player(1).bindings().set<MoveDownIntent>(1, game.input_manager().get_device(1)->get("Down"));
   game.get_player(1).bindings().set<SelectMenuItemIntent>(1, game.input_manager().get_device(1)->get("Return"));

   // create title text entities
   Entity* title = this->create_entity("Title Entity");
   title->add<Text>("TitleText", "SFML TEST", this->fonts().get("retro"), 36);
   title->get<Text>()->color(Color(sf::Color::White));

   // center text
   title->get<Text>()->origin(
      title->get<Text>()->local_bounds().left + title->get<Text>()->local_bounds().width / 2.f,
      title->get<Text>()->local_bounds().top + title->get<Text>()->local_bounds().height/ 2.f
   );

   title->get<Text>()->position(0, 0); // must position after changing origin

   Entity* menu_item_1 = this->create_entity("MenuItem_BuilderScene");
   menu_item_1->add<Text>("MenuItem_BuilderScene_Text", "Builder Scene", this->fonts().get("retro"), 12);
   menu_item_1->get<Text>()->color(Color(sf::Color::White));

   menu_item_1->get<Text>()->origin(
      menu_item_1->get<Text>()->local_bounds().left + menu_item_1->get<Text>()->local_bounds().width / 2.f,
      menu_item_1->get<Text>()->local_bounds().top + menu_item_1->get<Text>()->local_bounds().height/ 2.f
   );

   menu_item_1->get<Text>()->position(0, 60);

   Entity* menu_item_2 = this->create_entity("MenuItem_MegamanScene");
   menu_item_2->add<Text>("MenuItem_MegamanScene_Text", "Megaman Scene", this->fonts().get("retro"), 12);
   menu_item_2->get<Text>()->color(Color(sf::Color::White));

   menu_item_2->get<Text>()->origin(
      menu_item_2->get<Text>()->local_bounds().left + menu_item_2->get<Text>()->local_bounds().width / 2.f,
      menu_item_2->get<Text>()->local_bounds().top + menu_item_2->get<Text>()->local_bounds().height/ 2.f
   );

   menu_item_2->get<Text>()->position(0, 90);

   Entity* menu_item_3 = this->create_entity("MenuItem_PokemonScene");
   menu_item_3->add<Text>("MenuItem_PokemonScene_Text", "Pokemon Scene", this->fonts().get("retro"), 12);
   menu_item_3->get<Text>()->color(Color(sf::Color::White));

   menu_item_3->get<Text>()->origin(
      menu_item_3->get<Text>()->local_bounds().left + menu_item_3->get<Text>()->local_bounds().width / 2.f,
      menu_item_3->get<Text>()->local_bounds().top + menu_item_3->get<Text>()->local_bounds().height/ 2.f
   );

   menu_item_3->get<Text>()->position(0, 120);

   // create menu cursor
   Entity* cursor = this->create_entity("CursorEntity");
   cursor->add<PlayerProfile>("CursorPlayerProfile", 1);

   cursor->add<Sprite>("CursorSprite", this->textures().get("cursor"));
   cursor->get<Sprite>()->position(menu_item_1->get<Text>()->position() - sf::Vector2f(30, 7));
   cursor->get<Sprite>()->scale(3, 3);

   cursor->add<Callback>("CursorCallback");
   cursor->get<Callback>()->on_update([this, cursor, &game, menu_item_1, menu_item_2, menu_item_3] () {
      bool was_changed = false;
      InputBinding& p1_bindings = game.get_player(1).bindings();
      
      if (p1_bindings.get<MoveUpIntent>()->element()->was_pressed()) {
         --this->menu_select_;
         if (this->menu_select_ < 0) {
            this->menu_select_ = 2;
         }
         was_changed = true;
      }

      if (p1_bindings.get<MoveDownIntent>()->element()->was_pressed()) {
         this->menu_select_ = (this->menu_select_ + 1) % 3;
         was_changed = true;
      }

      if (was_changed) {
         switch (this->menu_select_) {
         case 0:
            cursor->get<Sprite>()->position(menu_item_1->get<Text>()->position() - sf::Vector2f(30, 7));
         break;
         case 1:
            cursor->get<Sprite>()->position(menu_item_2->get<Text>()->position() - sf::Vector2f(30, 7));
         break;
         case 2:
            cursor->get<Sprite>()->position(menu_item_3->get<Text>()->position() - sf::Vector2f(30, 7));
         break;
         default:
         break;
         }
      }

      if (p1_bindings.get<SelectMenuItemIntent>()->element()->was_pressed()) {
         switch (this->menu_select_) {
         case 0:
            this->switch_scene(new BuilderScene());
         break;
         case 1:
         break;
         case 2:
         break;
         default:
         break;
         }
      }
   });

   // center text entities in the middle of the screen
   Entity* text_layer = this->create_entity("Text Root");
   text_layer->add<PlayerProfile>("RootPlayerProfile", 1);
   text_layer->add<Callback>("RootCallback");
   
   // TODO: create a better way to get Systems
   GraphicalSystem* gs = dynamic_cast<GraphicalSystem*>(this->get_system("GraphicalSystem"));

   text_layer->get<Callback>()->camera_resize([text_layer, gs] () {
      text_layer->get<Space>()->position(gs->camera()->center() - sf::Vector2f(0, 30));
   });

   this->send_message<AddToEntityMessage>(text_layer->handle(), title->handle());
   this->send_message<AddToEntityMessage>(text_layer->handle(), menu_item_1->handle());
   this->send_message<AddToEntityMessage>(text_layer->handle(), menu_item_2->handle());
   this->send_message<AddToEntityMessage>(text_layer->handle(), menu_item_3->handle());
   this->send_message<AddToEntityMessage>(text_layer->handle(), cursor->handle());
}

void StartMenuScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering game start menu state.");
}

void StartMenuScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting game start menu state.");
}
