#include "PlayerInputSystem.h"
#include "Game.h"
#include "Logger.h"
#include "Input.h"
#include "InputElement.h"
#include "Rectangle.h"
#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"

PlayerInputSystem::PlayerInputSystem(const std::string& id /* = "PlayerInputSystem" */) {
}

PlayerInputSystem::~PlayerInputSystem() {
}

void PlayerInputSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Input, Rectangle>();
}

void PlayerInputSystem::on_update(Game& game, Entity& e) {
   InputElementPtr move_left_button = e.get<Input>()->get<MoveLeftIntent>()->element();
   InputElementPtr move_right_button = e.get<Input>()->get<MoveRightIntent>()->element();

   if(move_left_button && move_left_button->is_pressed()) {
      Game::logger().msg(this->id(), Logger::INFO, "MoveLeftIntent is active.");

      // shrink rectangle width
      if (e.get<Rectangle>()->size().x > 30) {
         sf::Vector2f new_size = e.get<Rectangle>()->size();
         new_size.x -= 10;

         e.get<Rectangle>()->size(new_size);
      }
   }

   if(move_right_button && move_right_button->is_pressed()) {
      Game::logger().msg(this->id(), Logger::INFO, "MoveRightIntent is active.");

      // enlarge rectangle width
      if (e.get<Rectangle>()->size().x < 1000) {
         sf::Vector2f new_size = e.get<Rectangle>()->size();
         new_size.x += 10;

         e.get<Rectangle>()->size(new_size);
      }
   }
}
