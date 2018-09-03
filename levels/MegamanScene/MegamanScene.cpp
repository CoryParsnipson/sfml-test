#include "MegamanScene.h"

#include "Game.h"
#include "Entity.h"
#include "Animation.h"

#include "Space.h"
#include "Sprite.h"
#include "Callback.h"
#include "Collision.h"
#include "Velocity.h"
#include "Gravity.h"
#include "Rectangle.h"
#include "Acceleration.h"
#include "PlayerProfile.h"

#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"
#include "JumpIntent.h"
#include "VisualDebugIntent.h"

#include "VisualDebugSystem.h"
#include "PhysicsSystem.h"

#include "SetVisualDebugMessage.h"

MegamanScene::MegamanScene()
: Scene("MegamanScene")
, visual_debug_enable_(false)
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
   game.get_player(1).bindings().set<JumpIntent>(1, game.input_manager().get_device(1)->get("Z"));
   game.get_player(1).bindings().set<VisualDebugIntent>(1, game.input_manager().get_device(1)->get("D"));

   // setup custom systems
   this->add_system(new PhysicsSystem("PhysicsSystem"));

   // TODO: create a better way to get Systems
   GraphicalSystem* gs = dynamic_cast<GraphicalSystem*>(this->get_system("GraphicalSystem"));

   VisualDebugSystem* vds = new VisualDebugSystem("VisualDebugSystem", game.window(), gs->camera());
   vds->disable(); // start with this off
   this->add_system(vds);

   Entity* root = this->get_entity(this->space_handle());
   root->add<PlayerProfile>("RootPlayerProfile", 1);
   root->add<Callback>("RootCallback");
   root->get<Callback>()->on_update([this, &game] () {
      if (game.get_player(1).bindings().get<VisualDebugIntent>()->element()->was_pressed()) {
         this->send_message<SetVisualDebugMessage>(this->visual_debug_enable_);
         this->visual_debug_enable_ = !this->visual_debug_enable_;
      }
   });

   AnimationPtr stand_r = std::make_shared<Animation>("megaman_zero_stand_r", this->textures().get("megaman_zero_spritesheet"));
   stand_r->add(sf::IntRect(  0, 0, 40, 50), 120);
   stand_r->add(sf::IntRect( 40, 0, 40, 50), 9);
   stand_r->add(sf::IntRect( 80, 0, 40, 50), 30);
   stand_r->add(sf::IntRect(120, 0, 40, 50), 9);
   stand_r->add(sf::IntRect(160, 0, 40, 50), 9);
   stand_r->add(sf::IntRect(200, 0, 40, 50), 9);

   AnimationPtr stand_l = std::make_shared<Animation>("megaman_zero_stand_l", this->textures().get("megaman_zero_spritesheet"));
   stand_l->add(sf::IntRect( 40, 0, -40, 50), 120);
   stand_l->add(sf::IntRect( 80, 0, -40, 50), 9);
   stand_l->add(sf::IntRect(120, 0, -40, 50), 30);
   stand_l->add(sf::IntRect(160, 0, -40, 50), 9);
   stand_l->add(sf::IntRect(200, 0, -40, 50), 9);
   stand_l->add(sf::IntRect(240, 0, -40, 50), 9);

   AnimationPtr start_run_r = std::make_shared<Animation>("megaman_zero_start_run_r", this->textures().get("megaman_zero_spritesheet"));
   start_run_r->add(sf::IntRect(  0, 50, 40, 50), 5);

   AnimationPtr run_r = std::make_shared<Animation>("megaman_zero_run_r", this->textures().get("megaman_zero_spritesheet"));
   run_r->add(sf::IntRect( 40, 50, 40, 50), 5);
   run_r->add(sf::IntRect( 80, 50, 40, 50), 5);
   run_r->add(sf::IntRect(120, 50, 40, 50), 5);
   run_r->add(sf::IntRect(160, 50, 40, 50), 5);
   run_r->add(sf::IntRect(200, 50, 40, 50), 5);
   run_r->add(sf::IntRect(240, 50, 40, 50), 5);
   run_r->add(sf::IntRect(280, 50, 40, 50), 5);
   run_r->add(sf::IntRect(320, 50, 40, 50), 5);
   run_r->add(sf::IntRect(360, 50, 40, 50), 5);
   run_r->add(sf::IntRect(400, 50, 40, 50), 5);

   AnimationPtr start_run_l = std::make_shared<Animation>("megaman_zero_start_run_l", this->textures().get("megaman_zero_spritesheet"));
   start_run_l->add(sf::IntRect(  40, 50, -40, 50), 5);
   
   AnimationPtr run_l = std::make_shared<Animation>("megaman_zero_run_l", this->textures().get("megaman_zero_spritesheet"));
   run_l->add(sf::IntRect( 80, 50, -40, 50), 5);
   run_l->add(sf::IntRect(120, 50, -40, 50), 5);
   run_l->add(sf::IntRect(160, 50, -40, 50), 5);
   run_l->add(sf::IntRect(200, 50, -40, 50), 5);
   run_l->add(sf::IntRect(240, 50, -40, 50), 5);
   run_l->add(sf::IntRect(280, 50, -40, 50), 5);
   run_l->add(sf::IntRect(320, 50, -40, 50), 5);
   run_l->add(sf::IntRect(360, 50, -40, 50), 5);
   run_l->add(sf::IntRect(400, 50, -40, 50), 5);
   run_l->add(sf::IntRect(440, 50, -40, 50), 5);

   AnimationPtr jump_r = std::make_shared<Animation>("megaman_zero_jump_r", this->textures().get("megaman_zero_spritesheet"));
   jump_r->add(sf::IntRect(  0, 100, 40, 50), 5);
   jump_r->add(sf::IntRect( 40, 100, 40, 50), 5);
   jump_r->add(sf::IntRect( 80, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(120, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(160, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(200, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(240, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(280, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(320, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(360, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(400, 100, 40, 50), 5);
   jump_r->add(sf::IntRect(440, 100, 40, 50), 5);

   Entity* test_e = this->create_entity("TestEntity");
   test_e->add<Sprite>("TestSprite");
   test_e->get<Sprite>()->animation(jump_r);
   test_e->get<Sprite>()->scale(3, 3);

   test_e->get<Space>()->position(100, 100);

   Entity* e_floor = this->create_entity("FloorEntity");
   e_floor->add<Rectangle>("FloorSprite", 0, 0, 600, 50);
   e_floor->get<Rectangle>()->color(sf::Color::Green);

   e_floor->get<Space>()->position(125, 500);

   e_floor->add<Collision>("FloorCollision", sf::FloatRect(0, 0, 600, 50));
   e_floor->add<Velocity>("FloorVelocity");
   e_floor->add<Acceleration>("FloorAcceleration");

   Entity* c = this->create_entity("PlayerCharacterEntity");
   c->add<Sprite>("PlayerCharacterSprite");
   c->get<Sprite>()->animation(stand_r);

   c->get<Space>()->position(300, 300);
   c->get<Space>()->scale(3, 3);

   c->add<Collision>("PlayerCharacterCollision", sf::FloatRect(0, 0, 40, 50));
   c->add<Velocity>("PlayerCharacterVelocity");
   c->add<Acceleration>("PlayerCharacterAcceleration");

   c->add<Gravity>("PlayerCharacterGravity", sf::Vector2f(0, 1));

   c->add<PlayerProfile>("PlayerCharacterPlayerProfile", 1);

   c->add<Callback>("PlayerCharacterCallback");
   c->get<Callback>()->on_update([this, &game, c, start_run_l, start_run_r, run_r, run_l, stand_r, stand_l, jump_r] () {
      InputBinding& bindings = game.get_player(1).bindings();
      
      // NOTE: The real thing should probably be done with an animation state machine somehow

      if (bindings.get<MoveLeftIntent>()->element()->was_pressed()) {
         // run transition animation for 1 frame
         c->get<Sprite>()->animation(start_run_l);
      } else if (bindings.get<MoveLeftIntent>()->element()->is_pressed()) {
         if (c->get<Sprite>()->animation() != run_l) {
            c->get<Sprite>()->animation(run_l);
         }
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

      // handle movement
      if (bindings.get<MoveLeftIntent>()->element()->is_pressed()) {
         c->get<Space>()->move(-5, 0);
      }

      if (bindings.get<MoveRightIntent>()->element()->is_pressed()) {
         c->get<Space>()->move(5, 0);
      }

      // experimental jumping code
      if (bindings.get<JumpIntent>()->element()->was_pressed()) {
         c->get<Space>()->move(0, -5); // hack...
         c->get<Velocity>()->y(c->get<Velocity>()->y() - 20);
      }
   });
}

void MegamanScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering MegamanScene.");
}

void MegamanScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting MegamanScene.");
}
