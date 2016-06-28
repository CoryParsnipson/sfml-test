#include "PlayerGamepad.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "Command.h"

//#include "InputEvent.h"
//#include "CloseInputEvent.h"
//#include "ResizeInputEvent.h"
//#include "KeyPressInputEvent.h"
//#include "MouseMoveInputEvent.h"
//#include "MouseWheelInputEvent.h"
//#include "MouseButtonInputEvent.h"

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
}

void PlayerGamepad::process(MouseWheelInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(MouseButtonInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));
}
