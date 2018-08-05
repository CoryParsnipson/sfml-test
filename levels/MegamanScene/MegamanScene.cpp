#include "MegamanScene.h"

#include "Game.h"
#include "Entity.h"
#include "Animation.h"

#include "Space.h"
#include "Sprite.h"
#include "Callback.h"
#include "PlayerProfile.h"

#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"

MegamanScene::MegamanScene()
: Scene("MegamanScene")
{
}

MegamanScene::~MegamanScene() {
}

void MegamanScene::init(Game& game) {
   // load fonts
   this->fonts().load("retro", "retro.ttf");

   // load textures
   this->textures().load("megaman_zero_spritesheet", "megaman_zero.png");

   // setup player's input mapping
   game.add_player(1);

   game.get_player(1).bindings().set<MoveLeftIntent>(1, game.input_manager().get_device(1)->get("Left"));
   game.get_player(1).bindings().set<MoveRightIntent>(1, game.input_manager().get_device(1)->get("Right"));

   AnimationPtr stand_r = std::make_shared<Animation>("megaman_zero_stand_r", this->textures().get("megaman_zero_spritesheet"));
   stand_r->add(sf::IntRect(  0, 0, 40, 41), 120);
   stand_r->add(sf::IntRect( 40, 0, 40, 41), 9);
   stand_r->add(sf::IntRect( 80, 0, 40, 41), 30);
   stand_r->add(sf::IntRect(120, 0, 40, 41), 9);
   stand_r->add(sf::IntRect(160, 0, 40, 41), 9);
   stand_r->add(sf::IntRect(200, 0, 40, 41), 9);

   AnimationPtr stand_l = std::make_shared<Animation>("megaman_zero_stand_l", this->textures().get("megaman_zero_spritesheet"));
   stand_l->add(sf::IntRect( 40, 0, -40, 41), 120);
   stand_l->add(sf::IntRect( 80, 0, -40, 41), 9);
   stand_l->add(sf::IntRect(120, 0, -40, 41), 30);
   stand_l->add(sf::IntRect(160, 0, -40, 41), 9);
   stand_l->add(sf::IntRect(200, 0, -40, 41), 9);
   stand_l->add(sf::IntRect(240, 0, -40, 41), 9);

   AnimationPtr start_run_r = std::make_shared<Animation>("megaman_zero_start_run_r", this->textures().get("megaman_zero_spritesheet"));
   start_run_r->add(sf::IntRect(  0, 41, 40, 41), 5);

   AnimationPtr run_r = std::make_shared<Animation>("megaman_zero_run_r", this->textures().get("megaman_zero_spritesheet"));
   run_r->add(sf::IntRect( 40, 41, 40, 41), 5);
   run_r->add(sf::IntRect( 80, 41, 40, 41), 5);
   run_r->add(sf::IntRect(120, 41, 40, 41), 5);
   run_r->add(sf::IntRect(160, 41, 40, 41), 5);
   run_r->add(sf::IntRect(200, 41, 40, 41), 5);
   run_r->add(sf::IntRect(240, 41, 40, 41), 5);
   run_r->add(sf::IntRect(280, 41, 40, 41), 5);
   run_r->add(sf::IntRect(320, 41, 40, 41), 5);
   run_r->add(sf::IntRect(360, 41, 40, 41), 5);
   run_r->add(sf::IntRect(400, 41, 40, 41), 5);

   AnimationPtr start_run_l = std::make_shared<Animation>("megaman_zero_start_run_l", this->textures().get("megaman_zero_spritesheet"));
   start_run_l->add(sf::IntRect(  40, 41, -40, 41), 5);
   
   AnimationPtr run_l = std::make_shared<Animation>("megaman_zero_run_l", this->textures().get("megaman_zero_spritesheet"));
   run_l->add(sf::IntRect( 80, 41, -40, 41), 5);
   run_l->add(sf::IntRect(120, 41, -40, 41), 5);
   run_l->add(sf::IntRect(160, 41, -40, 41), 5);
   run_l->add(sf::IntRect(200, 41, -40, 41), 5);
   run_l->add(sf::IntRect(240, 41, -40, 41), 5);
   run_l->add(sf::IntRect(280, 41, -40, 41), 5);
   run_l->add(sf::IntRect(320, 41, -40, 41), 5);
   run_l->add(sf::IntRect(360, 41, -40, 41), 5);
   run_l->add(sf::IntRect(400, 41, -40, 41), 5);
   run_l->add(sf::IntRect(440, 41, -40, 41), 5);

   Entity* c = this->create_entity("PlayerCharacterEntity");
   c->add<Sprite>("PlayerCharacterSprite");
   c->get<Sprite>()->animation(stand_r);
   c->get<Sprite>()->scale(3, 3);

   c->get<Space>()->position(300, 300);

   c->add<PlayerProfile>("PlayerCharacterPlayerProfile", 1);

   c->add<Callback>("PlayerCharacterCallback");
   c->get<Callback>()->on_update([this, &game, c, start_run_l, start_run_r, run_r, run_l, stand_r, stand_l] () {
      InputBinding& bindings = game.get_player(1).bindings();
      
      // NOTE: The real thing should probably be done with an animation state machine somehow

      if (bindings.get<MoveLeftIntent>()->element()->was_pressed()) {
         // run transition animation for 1 frame
         c->get<Sprite>()->animation(start_run_l);
      } else if (bindings.get<MoveLeftIntent>()->element()->is_pressed()) {
         c->get<Sprite>()->animation(run_l);
      } else if (bindings.get<MoveLeftIntent>()->element()->was_released()) {
         c->get<Sprite>()->animation(stand_l);
      }

      if (bindings.get<MoveRightIntent>()->element()->was_pressed()) {
         // run transition animation for 1 frame
         c->get<Sprite>()->animation(start_run_r);
      } else if (bindings.get<MoveRightIntent>()->element()->is_pressed()) {
         if (c->get<Sprite>()->animation() != run_r) {
            c->get<Sprite>()->animation(run_r);
         }
      } else if (bindings.get<MoveRightIntent>()->element()->was_released()) {
         c->get<Sprite>()->animation(stand_r);
      }
   });
}

void MegamanScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering MegamanScene.");
}

void MegamanScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting MegamanScene.");
}
