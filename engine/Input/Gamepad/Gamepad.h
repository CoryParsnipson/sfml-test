#ifndef GAMEPAD_H 
#define GAMEPAD_H

#include <string>

#include "SceneObject.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// Gamepad abstract base
//
// This class is an abstract base class that allows one to control game
// entities with commands. The abstract base class is also an input listener
// that does nothing by default. This class is also a SceneObject.
// ----------------------------------------------------------------------------
class Gamepad
: public SceneObject
, public InputListener
{
public:
   Gamepad(std::string id) : SceneObject(id), enable_(true) {}
   virtual ~Gamepad() {}

   operator std::string() const { return this->to_string(); }

   // gamepad interface
   bool is_enabled() { return this->enable_; }
   void enable() { this->enable_ = true; }
   void disable() { this->enable_ = false; }

   sf::Vector2f cursor_position() { return this->cursor_pos_; }
   sf::Vector2f prev_cursor_position() { return this->cursor_pos_prev_; }

   float wheel_delta() { return this->wheel_delta_; }

   // scene graph interface
   virtual bool intersects(const sf::Vector2i& other) {
      return this->intersects(static_cast<sf::Vector2f>(other));
   }

   virtual bool intersects(const sf::Vector2f& other) {
      return this->cursor_pos_ == this->global_transform().transformPoint(other);
   }

   virtual bool intersects(const sf::FloatRect& other) {
      return this->global_transform().transformRect(other).contains(this->cursor_pos_);
   }

   virtual bool intersects(const SceneObject& other) {
      // TODO: implement me
      return false;
   }

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor) {
      visitor.visit(this);
   }

   // input event processing
   virtual void process(CloseInputEvent& e) {}
   virtual void process(ResizeInputEvent& e) {}
   virtual void process(KeyPressInputEvent& e) {}
   virtual void process(KeyReleaseInputEvent& e) {}
   virtual void process(MouseMoveInputEvent& e) {}
   virtual void process(MouseWheelInputEvent& e) {}
   virtual void process(MouseButtonInputEvent& e) {}

   // serializable interface
   virtual std::string serialize(Serializer& s) = 0;
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d) = 0;

private:
   friend std::ostream& operator<<(std::ostream& stream, const Gamepad& gamepad) {
      stream << gamepad.to_string();
      return stream;
   }
   std::string to_string() const { return "[Gamepad: " + this->id() + "]"; }

protected:
   bool enable_;

   sf::Vector2f cursor_pos_;
   sf::Vector2f cursor_pos_prev_;

   float wheel_delta_;
};

#endif
