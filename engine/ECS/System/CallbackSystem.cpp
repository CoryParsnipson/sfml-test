#include <cassert>

#include "CallbackSystem.h"
#include "Game.h"
#include "Callback.h"
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

CallbackSystem::CallbackSystem(const std::string& id /* = "CallbackSystem" */)
: System(id, new PostorderEntitySubscription(id + "EntitySubscription"))
, camera_was_resized_(false)
, click_target_found_(false)
{
}

CallbackSystem::~CallbackSystem() {
}

void CallbackSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Callback, PlayerProfile>();

   // handle resize messages
   this->mailbox().handle<ResizeCameraMessage>([this](ResizeCameraMessage& msg) {
      this->camera_was_resized_ = true;
   });
}

void CallbackSystem::pre_update(Game& game) {
   this->click_target_found_ = false;
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

   prev_pos = e.get<Callback>()->prev_mouse_pos();
   if (bindings.get<MouseXIntent>() && bindings.get<MouseYIntent>()) {
      new_pos.x = bindings.get<MouseXIntent>()->element()->position();
      new_pos.y = bindings.get<MouseYIntent>()->element()->position();
      callback->prev_mouse_pos(new_pos);

      bool contains_new = collision->contains(new_pos);
      bool contains_old = collision->contains(prev_pos);

      // mouse wheel calculation
      if (collision && clickable && contains_new && bindings.get<MouseWheelIntent>() && !this->click_target_found_ &&
          bindings.get<MouseWheelIntent>()->element()->position() != callback->prev_mouse_wheel_pos()) {
         this->click_target_found_ = true;

         callback->prev_mouse_wheel_pos(bindings.get<MouseWheelIntent>()->element()->position());
         callback->mouse_wheel();
      }

      // left click calculation
      if (collision && clickable && contains_new && !this->click_target_found_ &&
          bindings.get<LeftClickIntent>()->element()->is_pressed() && !clickable->is_left_clicked()) {
         this->click_target_found_ = true;

         clickable->is_left_clicked(true);
         clickable->left_click_pos(new_pos);

         callback->left_click();
      }

      // right click calculation
      if (collision && clickable && contains_new && !this->click_target_found_ &&
          bindings.get<RightClickIntent>()->element()->is_pressed() && !clickable->is_right_clicked()) {
         this->click_target_found_ = true;

         clickable->is_right_clicked(true);
         clickable->right_click_pos(new_pos);

         callback->right_click();
      }
      
      // left release calculation (release all clicked elements no matter collision)
      if (clickable &&
          !bindings.get<LeftClickIntent>()->element()->is_pressed() && clickable->is_left_clicked()) {
         clickable->is_left_clicked(false);
         callback->left_release();
      }
      
      // right release calculation (release all clicked elements no matter collision)
      if (clickable &&
          !bindings.get<RightClickIntent>()->element()->is_pressed() && clickable->is_right_clicked()) {
         clickable->is_right_clicked(false);
         callback->right_release();
      }

      // mouse in calculation
      if (collision && contains_new && !contains_old) {
         callback->mouse_in();
      }

      // mouse out calculation
      if (collision && !contains_new && contains_old) {
         callback->mouse_out();
      }
      
      // mouse_move calculation
      if (new_pos != prev_pos) {
         callback->mouse_move();
      }
   }

   if (this->camera_was_resized_) {
      callback->camera_resize();

      // reset flag
      this->camera_was_resized_ = false;
   }
}
