#ifndef PLAYER_GAMEPAD_H
#define PLAYER_GAMEPAD_H

#include "dependencies.h"
#include "Input.h"
#include "Gamepad.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Command;

// ----------------------------------------------------------------------------
// PlayerGamepad class
//
// This is the base class that should be used for any gamepad implementation
// that requires user input (e.g. Mouse or keyboard control). This gamepad
// ----------------------------------------------------------------------------
class PlayerGamepad
: public Gamepad
{
public:
   using Binding = std::map<ButtonState, Command*>;
   using KeyBinding = std::map<Key, Binding>;
   using MouseBinding = std::map<MouseButton, Binding>;

   PlayerGamepad(std::string id = "PlayerGamepad");
   virtual ~PlayerGamepad();

   sf::Vector2f position();

   // command interface
   void set(Command* command, Key keycode, ButtonState state = ButtonState::Pressed);
   void set(Command* command, MouseButton button, ButtonState state = ButtonState::Pressed);

   void unset(Key keycode, ButtonState state);
   void unset(MouseButton button, ButtonState state);

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
   KeyBinding keys_;
   MouseBinding mouse_buttons_;
   Command* mouse_move_command_;
   Command* mouse_wheel_command_;

   sf::Vector2f pos_;
};

#endif
