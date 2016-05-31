#ifndef KEYBOARD_GAMEPAD_H
#define KEYBOARD_GAMEPAD_H

#include <map>

#include "Input.h"
#include "PlayerGamepad.h"

// ----------------------------------------------------------------------------
// foward declarations
// ----------------------------------------------------------------------------
class Command;

// ----------------------------------------------------------------------------
// KeyboardGamepad
// 
// This is a player gamepad that responds to keyboard input.
// ----------------------------------------------------------------------------
class KeyboardGamepad
: public PlayerGamepad
{
public:
   using Keybinding = std::map<Key, Command*>;
   
   KeyboardGamepad(std::string id = "KeyboardGamepad");
   virtual ~KeyboardGamepad();

   void bind(Key code, Command* command);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   // input event processing
   virtual void process(CloseInputEvent& e);
   virtual void process(ResizeInputEvent& e);
   virtual void process(KeyPressInputEvent& e);
   virtual void process(MouseMoveInputEvent& e);
   virtual void process(MouseWheelInputEvent& e);
   virtual void process(MouseButtonInputEvent& e);

protected:
   Keybinding config_;
};

#endif
