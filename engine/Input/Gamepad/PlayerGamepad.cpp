#include <cassert>

#include "PlayerGamepad.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "Command.h"
#include "TextFactory.h"

PlayerGamepad::PlayerGamepad(std::string id /* = "PlayerGamepad" */, sf::Font* cursor_font /* = nullptr */)
: Gamepad(id)
, mouse_move_command_(new NullCommand())
, mouse_wheel_command_(new NullCommand())
, show_cursor_(true)
, cursor_(new SpriteGraphic())
, updated_in_this_tick_(false)
{
   this->cursor_->set_size(6, 6);
   this->cursor_->set_color(sf::Color::Red);

   this->cursor_text_ = TextFactory::inst()->create_text("", cursor_font);

   // iterate through all possible keybindings
   for (int i = 0; i <= static_cast<int>(Key::Pause); ++i) {
      this->keys_[static_cast<Key>(i)] = new NullCommand();
   }
   
   // iterate through all possible mouse button combinations
   for (int i = 0; i <= static_cast<int>(MouseButton::XButton2); ++i) {
      this->mouse_buttons_[static_cast<MouseButton>(i)][ButtonState::Pressed] = new NullCommand();
      this->mouse_buttons_[static_cast<MouseButton>(i)][ButtonState::Released] = new NullCommand();
   }
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
   delete this->keys_[keycode];
   this->keys_[keycode] = command;
}

void PlayerGamepad::set(Command* command, MouseButton button, ButtonState state /* = ButtonState::Pressed */) {
   delete this->mouse_buttons_[button][state];
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
      Game::logger().msg(this->id(), Logger::ERROR, "Received invalid MouseAction directive.");
   break;
   }
}

void PlayerGamepad::unset(Key keycode) {
   delete this->keys_[keycode];
   this->keys_[keycode] = new NullCommand();
}

void PlayerGamepad::unset(MouseButton button, ButtonState state) {
   delete this->mouse_buttons_[button][state];
   this->mouse_buttons_[button][state] = new NullCommand();
}

void PlayerGamepad::unset(MouseAction binding) {
   switch (binding) {
   case MouseAction::Move:
      delete this->mouse_move_command_;
      this->mouse_move_command_ = new NullCommand();
   break;
   case MouseAction::Wheel:
      delete this->mouse_wheel_command_;
      this->mouse_wheel_command_ = new NullCommand();
   break;
   default:
      Game::logger().msg(this->id(), Logger::ERROR, "Received invalid MouseAction directive.");
   break;
   }
}

void PlayerGamepad::update(Game& game, Scene* scene /* = nullptr */) {
   sf::Vector2f c = this->cursor_position();

   this->cursor_->set_position(c);
   this->cursor_text_->set_position(c + this->cursor_->get_size());

   this->cursor_text_->set_string(std::to_string((int)c.x) + ", " + std::to_string((int)c.y));

   this->updated_in_this_tick_ = false;
}

void PlayerGamepad::process(CloseInputEvent& e) {
   Game::logger().msg(this->id(), Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(ResizeInputEvent& e) {
   Game::logger().msg(this->id(), Logger::INFO, "Received " + std::string(e));
}

void PlayerGamepad::process(KeyPressInputEvent& e) {
   Game::logger().msg(this->id(), Logger::INFO, "Received " + std::string(e));
   assert(this->keys_[e.key] != nullptr);
   this->keys_[e.key]->execute();
}

void PlayerGamepad::process(MouseMoveInputEvent& e) {
   Game::logger().msg(this->id(), Logger::INFO, "Received " + std::string(e));

   if (this->updated_in_this_tick_) {
      return;
   }

   // update gamepad position
   this->cursor_pos_prev_ = this->cursor_pos_;
   this->cursor_pos_ = sf::Vector2f(e.x, e.y);

   assert(this->mouse_move_command_ != nullptr);
   this->mouse_move_command_->execute();

   this->updated_in_this_tick_ = true;
}

void PlayerGamepad::process(MouseWheelInputEvent& e) {
   Game::logger().msg(this->id(), Logger::INFO, "Received " + std::string(e));

   // update gamepad wheel delta
   this->wheel_delta_ = e.delta;

   assert(this->mouse_wheel_command_ != nullptr);
   this->mouse_wheel_command_->execute();
}

void PlayerGamepad::process(MouseButtonInputEvent& e) {
   Game::logger().msg(this->id(), Logger::INFO, "Received " + std::string(e));

   // update gamepad position
   this->cursor_pos_prev_ = this->cursor_pos_;
   this->cursor_pos_ = sf::Vector2f(e.x, e.y);

   assert(this->mouse_buttons_[e.button][e.state] != nullptr);
   this->mouse_buttons_[e.button][e.state]->execute();
}

void PlayerGamepad::do_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   // transform to sceen coordinates 
   render_states.transform.translate(this->get_screen_coordinate(this->cursor_pos_) - this->cursor_pos_);

   this->cursor_->draw(surface, render_states);
   this->cursor_text_->draw(surface, render_states);
}

std::string PlayerGamepad::serialize(Serializer& s) {
   // TODO: implement me
   return "";
}

void PlayerGamepad::deserialize(Serializer& s, Scene& scene, std::string& d) {
   // TODO: implement me
}
