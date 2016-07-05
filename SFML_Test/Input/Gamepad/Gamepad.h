#ifndef GAMEPAD_H 
#define GAMEPAD_H

#include <string>

#include "Update.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// Gamepad abstract base
//
// This class is an abstract base class that allows one to control game
// entities with commands. The abstract base class is also an input listener
// that does nothing by default. 
// ----------------------------------------------------------------------------
class Gamepad
: public Update
, public InputListener
{
public:
   Gamepad(std::string id) : enable_(true), id_(std::move(id)) {}
   virtual ~Gamepad() {}

   operator std::string() const { return this->to_string(); }

   // gamepad interface
   bool is_enabled() { return this->enable_; }
   void enable() { this->enable_ = true; }
   void disable() { this->enable_ = false; }

   sf::Vector2f cursor_position() { return this->cursor_pos_; }
   sf::Vector2f prev_cursor_position() { return this->cursor_pos_prev_; }

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;

   // input event processing
   virtual void process(CloseInputEvent& e) {}
   virtual void process(ResizeInputEvent& e) {}
   virtual void process(KeyPressInputEvent& e) {}
   virtual void process(MouseMoveInputEvent& e) {}
   virtual void process(MouseWheelInputEvent& e) {}
   virtual void process(MouseButtonInputEvent& e) {}

private:
   friend std::ostream& operator<<(std::ostream& stream, const Gamepad& gamepad) {
      stream << gamepad.to_string();
      return stream;
   }
   std::string to_string() const { return "[Gamepad: " + this->id_ + "]"; }

protected:
   bool enable_;
   std::string id_;

   sf::Vector2f cursor_pos_;
   sf::Vector2f cursor_pos_prev_;
};

#endif
