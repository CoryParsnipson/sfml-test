#include "PlayerGamepad.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "Command.h"
#include "TextFactory.h"

PlayerGamepad::PlayerGamepad(std::string id /* = "PlayerGamepad" */, sf::Font* cursor_font /* = nullptr */)
: Gamepad(id)
, mouse_move_command_(nullptr)
, mouse_wheel_command_(nullptr)
, show_cursor_(true)
, cursor_(new SpriteGraphic())
{
   this->cursor_->set_size(6, 6);
   this->cursor_->set_color(sf::Color::Red);

   this->cursor_text_ = TextFactory::inst()->create_text("", cursor_font);
}

PlayerGamepad::~PlayerGamepad() {
   for (KeyBinding::iterator k_it = this->keys_.begin(); k_it != this->keys_.end(); ++k_it) {
      delete k_it->second;
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

   delete this->cursor_;
   delete this->cursor_text_;
}

void PlayerGamepad::set(Command* command, Key keycode) {
   this->unset(keycode);
   this->keys_[keycode] = command;
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

void PlayerGamepad::unset(Key keycode) {
   delete this->keys_[keycode];
   this->keys_[keycode] = nullptr;
}

void PlayerGamepad::unset(MouseButton button, ButtonState state) {
   delete this->mouse_buttons_[button][state];
   this->mouse_buttons_[button][state] = nullptr;
}

void PlayerGamepad::unset(MouseAction binding) {
   switch (binding) {
   case MouseAction::Move:
      delete this->mouse_move_command_;
      this->mouse_move_command_ = nullptr;
   break;
   case MouseAction::Wheel:
      delete this->mouse_wheel_command_;
      this->mouse_wheel_command_ = nullptr;
   break;
   default:
      Service::get_logger().msg(this->id_, Logger::ERROR, "Received invalid MouseAction directive.");
   break;
   }
}

void PlayerGamepad::update(Game& game, Scene* scene /* = nullptr */) {
   sf::Vector2f c = this->cursor_position();

   this->cursor_->set_position(c);
   this->cursor_text_->set_position(c + this->cursor_->get_size());

   this->cursor_text_->set_string(std::to_string((int)c.x) + ", " + std::to_string((int)c.y));
}

void PlayerGamepad::process(CloseInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(ResizeInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(KeyPressInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));

   Command* c = this->keys_[e.key];
   if (c) {
      c->execute();
   }
}

void PlayerGamepad::process(MouseMoveInputEvent& e) {
   Service::get_logger().msg(Gamepad::id_, Logger::INFO, "Received " + std::string(e));

   // update gamepad position
   this->cursor_pos_prev_ = this->cursor_pos_;
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
   this->cursor_pos_prev_ = this->cursor_pos_;
   this->cursor_pos_ = sf::Vector2f(e.x, e.y);

   Command* c = this->mouse_buttons_[e.button][e.state];
   if (c) {
      c->execute();
   }
}

void PlayerGamepad::do_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   // transform to sceen coordinates 
   render_states.transform.translate(this->get_screen_coordinate(this->cursor_pos_) - this->cursor_pos_);

   this->cursor_->draw(surface, render_states);
   this->cursor_text_->draw(surface, render_states);
}
