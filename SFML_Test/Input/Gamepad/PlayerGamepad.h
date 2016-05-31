#ifndef PLAYER_GAMEPAD_H
#define PLAYER_GAMEPAD_H

#include "Gamepad.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// PlayerGamepad abstract base
//
// This is the base class that should be used for any gamepad implementation
// that requires user input (e.g. Mouse or keyboard control). This gamepad
// is also an input listener, meaning that each player gamepad needs to be
// attached to an input subject to receive user input.
// ----------------------------------------------------------------------------
class PlayerGamepad
: public Gamepad
, public InputListener
{
public:
   PlayerGamepad(std::string id = "PlayerGamepad") : Gamepad(id) {}
   virtual ~PlayerGamepad() {}

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;

   // input event processing
   virtual void process(CloseInputEvent& e) = 0;
   virtual void process(ResizeInputEvent& e) = 0;
   virtual void process(KeyPressInputEvent& e) = 0;
   virtual void process(MouseMoveInputEvent& e) = 0;
   virtual void process(MouseWheelInputEvent& e) = 0;
   virtual void process(MouseButtonInputEvent& e) = 0;
};

#endif
