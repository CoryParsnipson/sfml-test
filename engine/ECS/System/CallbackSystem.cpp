#include <cassert>

#include "CallbackSystem.h"
#include "Game.h"
#include "Scene.h"
#include "Collision.h"
#include "Clickable.h"
#include "PlayerProfile.h"
#include "InputBinding.h"
#include "MouseXIntent.h"
#include "MouseYIntent.h"
#include "MouseWheelIntent.h"
#include "LeftClickIntent.h"
#include "RightClickIntent.h"

#include "PostorderEntitySubscription.h"

#include "ResizeCameraMessage.h"
#include "LeftClickMessage.h"
#include "LeftReleaseMessage.h"
#include "RightClickMessage.h"
#include "RightReleaseMessage.h"

CallbackSystem::CallbackSystem(const std::string& id /* = "CallbackSystem" */)
: System(id, new PostorderEntitySubscription(this, id + "EntitySubscription"))
, camera_was_resized_(false)
{
   this->target_hit_[CallbackKey::MOUSE_WHEEL] = false;
   this->target_hit_[CallbackKey::LEFT_CLICK] = false;
   this->target_hit_[CallbackKey::RIGHT_CLICK] = false;
}

CallbackSystem::~CallbackSystem() {
}

void CallbackSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Callback, PlayerProfile>();

   // handle resize messages
   this->install_message_handler<ResizeCameraMessage>([this](ResizeCameraMessage& msg) {
      this->camera_was_resized_ = true;
   });
}

void CallbackSystem::post_update(Game& game) {
   // execute deferred mouse_in events
   for (std::vector<Handle>::iterator it = this->mouse_in_entities_.begin(); it != this->mouse_in_entities_.end(); ++it) {
      Entity* e = this->scene().get_entity(*it);
      if (e && e->get<Callback>()) {
         e->get<Callback>()->mouse_in();
      }
   }
   this->mouse_in_entities_.clear();

   // reset mouse wheel target hit flags
   this->target_hit_[CallbackKey::MOUSE_WHEEL] = false;

   // reset flag
   this->camera_was_resized_ = false;
}

void CallbackSystem::on_update(Game& game, Entity& e) {
   sf::Vector2f prev_pos;
   sf::Vector2f new_pos;
   Callback* callback = e.get<Callback>();
   Collision* collision = e.get<Collision>();
   Clickable* clickable = e.get<Clickable>();
   Player::PlayerId pid = e.get<PlayerProfile>()->player_id();
   InputBinding& bindings(game.get_player(pid).bindings());

   assert(callback != nullptr);
   assert(e.get<PlayerProfile>() != nullptr);

   bool is_click_enabled = this->is_click_enabled(e);

   prev_pos = e.get<Callback>()->prev_mouse_pos();
   if (bindings.get<MouseXIntent>() && bindings.get<MouseYIntent>()) {
      new_pos.x = bindings.get<MouseXIntent>()->element()->position();
      new_pos.y = bindings.get<MouseYIntent>()->element()->position();

      bool contains_new = false;
      bool contains_old = false;
   
      if (collision) {
         sf::FloatRect global_collision_volume = this->global_transform(e).transformRect(collision->volume());

         contains_new = global_collision_volume.contains(new_pos);
         contains_old = global_collision_volume.contains(prev_pos);
      }

      // mouse wheel calculation
      if (contains_new &&
          bindings.get<MouseWheelIntent>() &&
          !this->target_hit_[CallbackKey::MOUSE_WHEEL]) {

         // set this to true no matter what the mouse wheel delta is, so mouse wheel events don't "bleed" through
         this->target_hit_[CallbackKey::MOUSE_WHEEL] = true;

         // only execute mouse wheel callback if the delta is non-zero
         if (bindings.get<MouseWheelIntent>()->element()->position() != callback->prev_mouse_wheel_pos()) {
            callback->mouse_wheel();

            // propagate this event upward in the scene graph
            Entity* parent_entity = e.space()->parent() ? game.current_scene()->get_entity(e.space()->parent()->entity()) : nullptr;
            bool propagate = callback->propagate();
            while (parent_entity && propagate) {
               if (parent_entity->get<Callback>()) {
                  parent_entity->get<Callback>()->mouse_wheel();
                  propagate = parent_entity->get<Callback>()->propagate();
               }

               parent_entity = parent_entity->space()->parent() ? game.current_scene()->get_entity(parent_entity->space()->parent()->entity()) : nullptr;
            }
         }
      }

      // update previous mouse wheel position (regardless of whether this mouse wheel event was triggered)
      if (bindings.get<MouseWheelIntent>()) {
         callback->prev_mouse_wheel_pos(bindings.get<MouseWheelIntent>()->element()->position());
      }

      // left click calculation
      if (is_click_enabled &&
          contains_new &&
          !this->target_hit_[CallbackKey::LEFT_CLICK] &&
          bindings.get<LeftClickIntent>() && 
          bindings.get<LeftClickIntent>()->element()->is_pressed() && !clickable->is_left_clicked()) {

         this->target_hit_[CallbackKey::LEFT_CLICK] = true;

         clickable->is_left_clicked(true);
         clickable->left_click_pos(new_pos);
         callback->left_click();

         // send out left click message
         if (!this->target_hit_[CallbackKey::LEFT_CLICK]) {
            this->send_message<LeftClickMessage>(new_pos);
         }

         // propagate this event upward in the scene graph
         Entity* parent_entity = e.space()->parent() ? game.current_scene()->get_entity(e.space()->parent()->entity()) : nullptr;
         bool propagate = callback->propagate();
         while (parent_entity && propagate) {
            if (parent_entity->get<Callback>() && parent_entity->get<Clickable>()) {
               parent_entity->get<Clickable>()->is_left_clicked(true);
               parent_entity->get<Clickable>()->left_click_pos(new_pos);

               parent_entity->get<Callback>()->left_click();

               if (!game.current_scene()->get_entity(parent_entity->handle())) {
                  // if parent entity is removed or scene graph is changed by callback, abort propagation
                  break;
               }
               propagate = parent_entity->get<Callback>()->propagate();
            }

            parent_entity = parent_entity->space()->parent() ? game.current_scene()->get_entity(parent_entity->space()->parent()->entity()) : nullptr;
         }
      }

      // right click calculation
      if (is_click_enabled &&
          contains_new &&
          !this->target_hit_[CallbackKey::RIGHT_CLICK] &&
          bindings.get<RightClickIntent>() && 
          bindings.get<RightClickIntent>()->element()->is_pressed() && !clickable->is_right_clicked()) {

         this->target_hit_[CallbackKey::RIGHT_CLICK] = true;

         clickable->is_right_clicked(true);
         clickable->right_click_pos(new_pos);
         callback->right_click();

         // send out right click message
         if (!this->target_hit_[CallbackKey::RIGHT_CLICK]) {
            this->send_message<RightClickMessage>(new_pos);
         }

         // propagate this event upward in the scene graph
         Entity* parent_entity = e.space()->parent() ? game.current_scene()->get_entity(e.space()->parent()->entity()) : nullptr;
         bool propagate = callback->propagate();
         while (parent_entity && propagate) {
            if (parent_entity->get<Callback>() && parent_entity->get<Clickable>()) {
               parent_entity->get<Clickable>()->is_right_clicked(true);
               parent_entity->get<Clickable>()->right_click_pos(new_pos);

               parent_entity->get<Callback>()->right_click();

               if (!game.current_scene()->get_entity(parent_entity->handle())) {
                  // if parent entity is removed or scene graph is changed by callback, abort propagation
                  break;
               }
               propagate = parent_entity->get<Callback>()->propagate();
            }

            parent_entity = parent_entity->space()->parent() ? game.current_scene()->get_entity(parent_entity->space()->parent()->entity()) : nullptr;
         }
      }
      
      // left release calculation (release all clicked elements no matter collision)
      if (is_click_enabled && 
          bindings.get<LeftClickIntent>() && 
          !bindings.get<LeftClickIntent>()->element()->is_pressed() &&
          clickable->is_left_clicked()) {

         clickable->is_left_clicked(false);
         callback->left_release();

         // send out left release message
         if (this->target_hit_[CallbackKey::LEFT_CLICK]) {
            this->send_message<LeftReleaseMessage>(clickable->left_click_pos(), new_pos);
         }
         this->target_hit_[CallbackKey::LEFT_CLICK] = false;

         // propagate this event upward in the scene graph
         Entity* parent_entity = e.space()->parent() ? game.current_scene()->get_entity(e.space()->parent()->entity()) : nullptr;
         bool propagate = callback->propagate();
         while (parent_entity && propagate) {
            if (parent_entity->get<Callback>() && parent_entity->get<Clickable>()) {
               parent_entity->get<Clickable>()->is_left_clicked(false);

               parent_entity->get<Callback>()->left_release();

               if (!game.current_scene()->get_entity(parent_entity->handle())) {
                  // if parent entity is removed or scene graph is changed by callback, abort propagation
                  break;
               }
               propagate = parent_entity->get<Callback>()->propagate();
            }

            parent_entity = parent_entity->space()->parent() ? game.current_scene()->get_entity(parent_entity->space()->parent()->entity()) : nullptr;
         }
      }
      
      // right release calculation (release all clicked elements no matter collision)
      if (is_click_enabled && 
          bindings.get<RightClickIntent>() && 
          !bindings.get<RightClickIntent>()->element()->is_pressed() &&
          clickable->is_right_clicked()) {

         clickable->is_right_clicked(false);
         callback->right_release();

         // send out right release message
         if (this->target_hit_[CallbackKey::RIGHT_CLICK]) {
            this->send_message<RightReleaseMessage>(clickable->right_click_pos(), new_pos);
         }

         this->target_hit_[CallbackKey::RIGHT_CLICK] = false;

         // propagate this event upward in the scene graph
         Entity* parent_entity = e.space()->parent() ? game.current_scene()->get_entity(e.space()->parent()->entity()) : nullptr;
         bool propagate = callback->propagate();
         while (parent_entity && propagate) {
            if (parent_entity->get<Callback>() && parent_entity->get<Clickable>()) {
               parent_entity->get<Clickable>()->is_right_clicked(false);

               parent_entity->get<Callback>()->right_release();

               if (!game.current_scene()->get_entity(parent_entity->handle())) {
                  // if parent entity is removed or scene graph is changed by callback, abort propagation
                  break;
               }
               propagate = parent_entity->get<Callback>()->propagate();
            }

            parent_entity = parent_entity->space()->parent() ? game.current_scene()->get_entity(parent_entity->space()->parent()->entity()) : nullptr;
         }
      }

      // mouse in calculation
      if (contains_new && !contains_old) {
          // defer this execution until after all mouse_out callbacks are run
          this->mouse_in_entities_.push_back(e.handle());
      }

      // mouse out calculation
      if (!contains_new && contains_old) {
         callback->mouse_out();
      }
      
      // mouse_move calculation
      if (new_pos != prev_pos) {
         callback->mouse_move();
      }

      // now update previous mouse position
      callback->prev_mouse_pos(new_pos);
   }

   // run this always
   callback->on_update();

   if (this->camera_was_resized_) {
      callback->camera_resize();
   }
}

bool CallbackSystem::is_click_enabled(Entity& e) {
   bool is_click_enabled = true;

   if (!e.get<Clickable>()) {
      return false;
   }

   Entity* current_entity = &e;
   while (current_entity && is_click_enabled) {
      if (current_entity->get<Clickable>()) {
         is_click_enabled &= current_entity->get<Clickable>()->is_enabled();
      }

      current_entity = current_entity->space()->parent() ?
         this->scene().get_entity(current_entity->space()->parent()->entity()) : nullptr;
   }

   return is_click_enabled;
}
