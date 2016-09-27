#ifndef PLAYER_GAMEPAD_H
#define PLAYER_GAMEPAD_H

#include <map>
#include <string>

#include "sfml.h"
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
   using KeyBinding = std::map<Key, Command*>;
   using MouseBinding = std::map<MouseButton, Binding>;

   PlayerGamepad(std::string id = "PlayerGamepad", sf::Font* cursor_font = nullptr);
   virtual ~PlayerGamepad();

   // command interface
   void set(Command* command, Key keycode);
   void set(Command* command, MouseButton button, ButtonState state = ButtonState::Pressed);
   void set(Command* command, MouseAction binding);

   void unset(Key keycode);
   void unset(MouseButton button, ButtonState state);
   void unset(MouseAction binding);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

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

   bool show_cursor_;
   Graphic* cursor_;
   Graphic* cursor_text_;

   // scene object interface hooks
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

private:
   bool updated_in_this_tick_;
};

#endif
