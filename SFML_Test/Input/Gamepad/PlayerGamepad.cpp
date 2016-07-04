#include "PlayerGamepad.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "Command.h"

PlayerGamepad::PlayerGamepad(std::string id /* = "PlayerGamepad" */)
: Gamepad(id)
, mouse_move_command_(nullptr)
, mouse_wheel_command_(nullptr)
{}

PlayerGamepad::~PlayerGamepad() {
   for (KeyBinding::iterator k_it = this->keys_.begin(); k_it != this->keys_.end(); ++k_it) {
      for (Binding::const_iterator b_it = k_it->second.begin(); b_it != k_it->second.end(); ++b_it) {
         delete b_it->second;
      }
      k_it->second.clear();
   }
   this->keys_.clear();

   for (MouseBinding::iterator m_it = this->mouse_buttons_.begin(); m_it != this->mouse_buttons_.end(); ++m_it) {
      for (Binding::const_iterator b_it = m_it->second.begin(); b_it != m_it->second.end(); ++b_it) {
         delete b_it->second;
      }
      m_it->second.clear();
   }
   this->mouse_buttons_.clear();

   delete this->mouse_move_command_;
   delete this->mouse_wheel_command_;
}

void PlayerGamepad::set(Command* command, Key keycode, ButtonState state /* = ButtonState::Pressed */) {
   this->unset(keycode, state);
   this->keys_[keycode][state] = command;
}

void PlayerGamepad::set(Command* command, MouseButton button, ButtonState state /* = ButtonState::Pressed */) {
   this->unset(button, state);
   this->mouse_buttons_[button][state] = command;
}

void PlayerGamepad::set(Command* command, MouseAction binding) {
   switch (binding) {
   case MouseAction::Move:
      delete this->mouse_move_command_;
      this->mouse_move_command_ = command;
   break;
   case MouseAction::Wheel:
      delete this->mouse_wheel_command_;
      this->mouse_wheel_command_ = command;
   break;
   default:
      Service::get_logger().msg(this->id_, Logger::ERROR, "Received invalid MouseAction directive.");
   break;
   }
}

void PlayerGamepad::unset(Key keycode, ButtonState state) {
   delete this->keys_[keycode][state];
   this->keys_[keycode][state] = nullptr;
}

void PlayerGamepad::unset(MouseButton button, ButtonState state) {
   delete this->mouse_buttons_[button][state];
   this->mouse_buttons_[button][state] = nullptr;
}

void PlayerGamepad::update(Game& game, Scene* scene /* = nullptr */, Entity* entity /* = nullptr */) {
   // empty
}

void PlayerGamepad::process(CloseInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(ResizeInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(KeyPressInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(MouseMoveInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));

   // update gamepad position
   this->cursor_pos_ = sf::Vector2f(e.x, e.y);

   if (this->mouse_move_command_) {
      this->mouse_move_command_->execute();
   }
}

void PlayerGamepad::process(MouseWheelInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(MouseButtonInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));

   // update gamepad position
   this->cursor_pos_ = sf::Vector2f(e.x, e.y);

   Command* c = this->mouse_buttons_[e.button][e.state];
   if (c) {
      c->execute();
   }
}
