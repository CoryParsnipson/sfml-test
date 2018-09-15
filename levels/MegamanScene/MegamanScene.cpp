#include "MegamanScene.h"

#include "Game.h"
#include "Entity.h"
#include "Animation.h"

#include "Space.h"
#include "Text.h"
#include "Sprite.h"
#include "Callback.h"
#include "Collision.h"
#include "Velocity.h"
#include "Gravity.h"
#include "Rectangle.h"
#include "Acceleration.h"
#include "PlayerProfile.h"
#include "VertexList.h"

#include "VisualDebugIntent.h"
#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"
#include "JumpIntent.h"
#include "StepIntent.h"

#include "VisualDebugSystem.h"
#include "PhysicsSystem.h"

#include "SetVisualDebugMessage.h"

MegamanScene::MegamanScene()
: Scene("MegamanScene")
, visual_debug_enable_(false)
, need_step_(false)
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
   game.get_player(1).bindings().set<StepIntent>(1, game.input_manager().get_device(1)->get("Escape"));
   game.get_player(1).bindings().set<VisualDebugIntent>(1, game.input_manager().get_device(1)->get("D"));

   // TODO: create a better way to get Systems
   GraphicalSystem* gs = dynamic_cast<GraphicalSystem*>(this->get_system("GraphicalSystem"));

   VisualDebugSystem* vds = new VisualDebugSystem("VisualDebugSystem", game.window(), gs->camera());
   vds->disable(); // start with this off
   this->add_system(vds);

   // setup custom systems
   PhysicsSystem* ps = new PhysicsSystem("PhysicsSystem");
   this->add_system(ps);

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

   test_e->get<Space>()->position(400, 100);

   Entity* e_floor = this->create_entity("FloorEntity");
   e_floor->add<Rectangle>("FloorSprite", 0, 0, 400, 50);
   e_floor->get<Rectangle>()->color(sf::Color::Green);

   e_floor->get<Space>()->position(200, 450);

   e_floor->add<Collision>("FloorCollision", sf::FloatRect(0, 0, 400, 50));
   e_floor->add<Velocity>("FloorVelocity");
   e_floor->add<Acceleration>("FloorAcceleration");

   Entity* c = this->create_entity("PlayerCharacterEntity");
   c->add<Sprite>("PlayerCharacterSprite");
   c->get<Sprite>()->animation(stand_r);

   c->get<Space>()->position(300, 100);
   //c->get<Space>()->scale(3, 3);

   c->add<Collision>("PlayerCharacterCollision", sf::FloatRect(0, 0, 40, 50));
   c->add<Velocity>("PlayerCharacterVelocity");
   c->add<Acceleration>("PlayerCharacterAcceleration");

   c->add<Gravity>("PlayerCharacterGravity", sf::Vector2f(0, 1));

   c->add<PlayerProfile>("PlayerCharacterPlayerProfile", 1);

   c->add<Callback>("PlayerCharacterCallback");
   c->get<Callback>()->on_update([this, &game, c, ps, start_run_l, start_run_r, run_r, run_l, stand_r, stand_l, jump_r] () {
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
         c->get<Velocity>()->x(-5);
      }

      if (bindings.get<MoveLeftIntent>()->element()->was_released() && !bindings.get<MoveRightIntent>()->element()->is_pressed()) {
         c->get<Velocity>()->x(0);
      }

      if (bindings.get<MoveRightIntent>()->element()->was_pressed()) {
         c->get<Velocity>()->x(5);
      }

      if (bindings.get<MoveRightIntent>()->element()->was_released() && !bindings.get<MoveLeftIntent>()->element()->is_pressed()) {
         c->get<Velocity>()->x(0);
      }

      // experimental jumping code
      if (bindings.get<JumpIntent>()->element()->was_pressed() && c->get<Velocity>()->y() == 0.f) {
         c->get<Space>()->move(0, -5); // hack...
         c->get<Velocity>()->y(c->get<Velocity>()->y() - 20);
      }

      if (this->need_step_) {
         ps->disable();
         this->need_step_ = false;
      }

      // stepping code
      if (bindings.get<StepIntent>()->element()->was_pressed()) {
         if (ps->is_enabled()) {
            ps->disable();
            this->need_step_ = false;
         } else {
            ps->enable();
            this->need_step_ = true;
         }

         Game::logger().msg("Stepper", Logger::INFO, "Stepping PhysicsSystem");
      }

      // draw physics system vectors
      Entity* dv1 = this->get_entity("DebugVector1");
      dv1->get<VertexList>()->vertex_color(0, sf::Color::White);
      dv1->get<VertexList>()->vertex_color(1, sf::Color::White);

      dv1->get<VertexList>()->vertex_position(0, ps->e_center);
      dv1->get<VertexList>()->vertex_position(1, ps->other_e_center);

      sf::Vector2f near_time_x_start;
      sf::Vector2f near_time_x_end;

      near_time_x_start.x = ps->e_center.x - ps->sign_x * ps->e_collision.width / 2.f;
      near_time_x_start.y = ps->e_center.y;

      near_time_x_end.x = ps->other_e_center.x - ps->sign_x * ps->other_e_collision.width / 2.f;
      near_time_x_end.y = ps->e_center.y;

      Entity* dv2 = this->get_entity("DebugVector2");
      dv2->get<VertexList>()->vertex_color(0, sf::Color::Red);
      dv2->get<VertexList>()->vertex_color(1, sf::Color::Red);

      dv2->get<VertexList>()->vertex_position(0, near_time_x_start);
      dv2->get<VertexList>()->vertex_position(1, near_time_x_end);

      sf::Vector2f near_time_y_start;
      sf::Vector2f near_time_y_end;

      near_time_y_start.x = ps->e_center.x;
      near_time_y_start.y = ps->e_center.y - ps->sign_y * ps->e_collision.height / 2.f;

      near_time_y_end.x = ps->e_center.x;
      near_time_y_end.y = ps->other_e_center.y - ps->sign_y * ps->other_e_collision.height / 2.f;

      Entity* dv3 = this->get_entity("DebugVector3");
      dv3->get<VertexList>()->vertex_color(0, sf::Color::Yellow);
      dv3->get<VertexList>()->vertex_color(1, sf::Color::Yellow);

      dv3->get<VertexList>()->vertex_position(0, near_time_y_start);
      dv3->get<VertexList>()->vertex_position(1, near_time_y_end);

      sf::Vector2f far_time_x_start, far_time_x_end;

      far_time_x_start.x = ps->e_center.x + ps->sign_x * ps->e_collision.width / 2.f;
      far_time_x_start.y = ps->e_center.y;

      far_time_x_end.x = ps->other_e_center.x + ps->sign_x * ps->other_e_collision.width / 2.f;
      far_time_x_end.y = ps->e_center.y;

      Entity* dv4 = this->get_entity("DebugVector4");
      dv4->get<VertexList>()->vertex_color(0, sf::Color::Cyan);
      dv4->get<VertexList>()->vertex_color(1, sf::Color::Cyan);

      dv4->get<VertexList>()->vertex_position(0, far_time_x_start);
      dv4->get<VertexList>()->vertex_position(1, far_time_x_end);

      sf::Vector2f far_time_y_start, far_time_y_end;

      far_time_y_start.x = ps->e_center.x;
      far_time_y_start.y =  ps->e_center.y + ps->sign_y * ps->e_collision.height / 2.f;

      far_time_y_end.x = ps->e_center.x;
      far_time_y_end.y = ps->other_e_center.y + ps->sign_y * ps->other_e_collision.height / 2.f;

      Entity* dv5 = this->get_entity("DebugVector5");
      dv5->get<VertexList>()->vertex_color(0, sf::Color::Magenta);
      dv5->get<VertexList>()->vertex_color(1, sf::Color::Magenta);

      dv5->get<VertexList>()->vertex_position(0, far_time_y_start);
      dv5->get<VertexList>()->vertex_position(1, far_time_y_end);
   });

   Entity* dv1 = this->create_entity("DebugVector1");
   Entity* dv2 = this->create_entity("DebugVector2");
   Entity* dv3 = this->create_entity("DebugVector3");
   Entity* dv4 = this->create_entity("DebugVector4");
   Entity* dv5 = this->create_entity("DebugVector5");

   dv1->add<VertexList>("DebugVectorVertexList", sf::LineStrip, 2);
   dv2->add<VertexList>("DebugVectorVertexList", sf::LineStrip, 2);
   dv3->add<VertexList>("DebugVectorVertexList", sf::LineStrip, 2);
   dv4->add<VertexList>("DebugVectorVertexList", sf::LineStrip, 2);
   dv5->add<VertexList>("DebugVectorVertexList", sf::LineStrip, 2);

   Entity* root = this->get_entity(this->space_handle());
   root->add<PlayerProfile>("RootPlayerProfile", 1);

   root->add<Text>("DiagnosticText", "", this->fonts().get("retro"), 12);
   root->get<Text>()->color(sf::Color::White);
   root->get<Text>()->offset(10, 10);

   root->add<Callback>("RootCallback");
   root->get<Callback>()->on_update([this, c, root, ps, &game] () {
      if (game.get_player(1).bindings().get<VisualDebugIntent>()->element()->was_pressed()) {
         this->visual_debug_enable_ = !this->visual_debug_enable_;
         this->send_message<SetVisualDebugMessage>(this->visual_debug_enable_);
      }

      std::string line;

      line  = "Velocity: (" + std::to_string(c->get<Velocity>()->x()) + ", " + std::to_string(c->get<Velocity>()->y()) + ")\n";
      line += "Position: (" + std::to_string((int)c->get<Space>()->position().x) + ", " + std::to_string((int)c->get<Space>()->position().y) + ")\n";
      line += "Collision time: " + std::to_string(ps->collision_time) + "\n";
      line += "normal: (" + std::to_string((int)ps->normal.x) + ", " + std::to_string((int)ps->normal.y) + ")\n";
      line += "overlap: (" + std::to_string((int)ps->overlap.x) + ", " + std::to_string((int)ps->overlap.y) + ")\n";
      line += "scale_x: " + std::to_string(ps->scale_x) + "\n";
      line += "scale_y: " + std::to_string(ps->scale_y) + "\n";
      line += "sign_x: " + std::to_string(ps->sign_x) + "\n";
      line += "sign_y: " + std::to_string(ps->sign_y) + "\n";
      line += "near_time_x: " + std::to_string(ps->near_time_x) + "\n";
      line += "near_time_y: " + std::to_string(ps->near_time_y) + "\n";
      line += "far_time_x: " + std::to_string(ps->far_time_x) + "\n";
      line += "far_time_y: " + std::to_string(ps->far_time_y) + "\n";
      line += "delta_x: " + std::to_string(ps->delta_x) + "\n";
      line += "delta_y: " + std::to_string(ps->delta_y) + "\n";
      line += "overlap_x: " + std::to_string(ps->overlap_x) + "\n";
      line += "overlap_y: " + std::to_string(ps->overlap_y) + "\n";
      line += "dotprod: " + std::to_string(ps->dotprod) + "\n";
      line += "e_center: (" + std::to_string((int)ps->e_center.x) + ", " + std::to_string((int)ps->e_center.y) + ")\n";
      line += "other_e_center: (" + std::to_string((int)ps->other_e_center.x) + ", " + std::to_string((int)ps->other_e_center.y) + ")\n";
      line += "e_collision: ( top -> " + std::to_string((int)ps->e_collision.top) + ", left -> " + std::to_string((int)ps->e_collision.left) + ", width -> " + std::to_string((int)ps->e_collision.width) + ", height -> " + std::to_string((int)ps->e_collision.height) + ")\n";
      line += "other_e_collision: ( top -> " + std::to_string((int)ps->other_e_collision.top) + ", left -> " + std::to_string((int)ps->other_e_collision.left) + ", width -> " + std::to_string((int)ps->other_e_collision.width) + ", height -> " + std::to_string((int)ps->other_e_collision.height) + ")\n";
      line += "Algorithm: " + ps->algorithm + "\n";
      line += "Physics enabled: " + std::string(ps->is_enabled() ? "ENABLED" : "DISABLED") + "\n";

      root->get<Text>()->string(line);
   });

}

void MegamanScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering MegamanScene.");
}

void MegamanScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting MegamanScene.");
}
