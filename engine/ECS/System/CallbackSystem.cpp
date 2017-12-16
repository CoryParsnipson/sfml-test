#include "CallbackSystem.h"
#include "Game.h"
#include "Callback.h"
#include "PlayerProfile.h"
#include "InputBinding.h"
#include "MouseXIntent.h"
#include "MouseYIntent.h"

CallbackSystem::CallbackSystem(const std::string& id /* = "CallbackSystem" */)
: System(id)
{
}

CallbackSystem::~CallbackSystem() {
}

void CallbackSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Callback, PlayerProfile>();
}

void CallbackSystem::on_update(Game& game, Entity& e) {
   sf::Vector2f prev_pos;
   sf::Vector2f new_pos;
   Player::PlayerId pid = e.get<PlayerProfile>()->player_id();
   InputBinding& bindings(game.get_player(pid).bindings());

   prev_pos = e.get<Callback>()->prev_mouse_pos();

   if (bindings.get<MouseXIntent>() && bindings.get<MouseYIntent>()) {
      new_pos.x = bindings.get<MouseXIntent>()->element()->position();
      new_pos.y = bindings.get<MouseXIntent>()->element()->position();
      e.get<Callback>()->prev_mouse_pos(new_pos);

      // mouse in calculation

      // mouse out calculation
      
      // mouse_move calculation
      if (new_pos != prev_pos) {
         e.get<Callback>()->mouse_move();
      }
   }
}
