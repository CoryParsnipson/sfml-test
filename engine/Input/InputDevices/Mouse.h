#ifndef MOUSE_H
#define MOUSE_H

#include "InputDevice.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"
#include "InputManager.h"

// ----------------------------------------------------------------------------
// Mouse
//
// This is an input device that should handle mouse behavior.
// ----------------------------------------------------------------------------
class Mouse : public InputDevice {
public:
   Mouse(const std::string& name = "Mouse") : InputDevice(name) {
      // add mouse buttons
      this->add_button("Left");
      this->add_button("Right");
      this->add_button("Middle");
      this->add_button("XButton1");
      this->add_button("XButton2");

      // add mouse axes
      this->add_axis("PositionX");
      this->add_axis("PositionY");
      this->add_axis("Wheel");
   }

   virtual ~Mouse() {}

   virtual void process(MouseMoveInputEvent& e) {
      this->get("PositionX")->position(e.x);
      this->get("PositionY")->position(e.y);
   }

   virtual void process(MouseWheelInputEvent& e) {
      this->get("PositionX")->position(e.x);
      this->get("PositionY")->position(e.y);
      
      this->get("Wheel")->position(e.delta);
   }

   virtual void process(MouseButtonInputEvent& e) {
      this->get("PositionX")->position(e.x);
      this->get("PositionY")->position(e.y);

      switch(e.button) {
      case MouseButton::Left:
         this->get("Left")->position(e.state == ButtonState::Pressed ? 1 : 0);
         break;
      case MouseButton::Right:
         this->get("Right")->position(e.state == ButtonState::Pressed ? 1 : 0);
         break;
      case MouseButton::Middle:
         this->get("Middle")->position(e.state == ButtonState::Pressed ? 1 : 0);
         break;
      case MouseButton::XButton1:
         this->get("XButton1")->position(e.state == ButtonState::Pressed ? 1 : 0);
         break;
      case MouseButton::XButton2:
         this->get("XButton2")->position(e.state == ButtonState::Pressed ? 1 : 0);
         break;
      default:
         Game::logger().msg(this->name(), Logger::WARNING,
            "Received MouseButtonInputEvent with unknown button encoding.");
      }
   }
};

#endif
