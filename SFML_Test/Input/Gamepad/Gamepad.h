#ifndef GAMEPAD_H 
#define GAMEPAD_H

#include <string>

#include "Update.h"

// ----------------------------------------------------------------------------
// Gamepad abstract base
//
// This class is an abstract base class that allows one to control game
// entities with commands.
// ----------------------------------------------------------------------------
class Gamepad
{
public:
   Gamepad(std::string id) : enable_(true), id_(std::move(id)) {}
   virtual ~Gamepad() {}

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;

   bool is_enabled() { return this->enable_; }
   void enable() { this->enable_ = true; }
   void disable() { this->enable_ = false; }
   
   operator std::string() const { return this->to_string(); }

private:
   friend std::ostream& operator<<(std::ostream& stream, const Gamepad& gamepad) {
      stream << gamepad.to_string();
      return stream;
   }
   std::string to_string() const { return "[Gamepad: " + this->id_ + "]"; }

protected:
   bool enable_;
   std::string id_;
};

#endif
