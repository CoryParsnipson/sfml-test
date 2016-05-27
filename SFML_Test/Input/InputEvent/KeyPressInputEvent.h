#ifndef KEY_PRESS_INPUT_EVENT_H
#define KEY_PRESS_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

enum class Key {
   A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
   Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
   Escape, LCtrl, LShift, LAlt, LSystem, RCtrl, RShift, RAlt, RSystem, Menu,
   LBracket, RBracket, SemiColon, Comma, Period, Quote, Slash, BackSlash, Tilde, Equal, Dash,
   Space, Return, Backspace, Tab,
   PageUp, PageDown, End, Home, Insert, Delete, Add, Subtract, Multiply, Divide,
   Left, Right, Up, Down,
   Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
   F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
   Pause
};

// ----------------------------------------------------------------------------
//  KeyPressInputEvent
//
//  This event should be emitted when the user presses a key on the keyboard.
//  The event contains a keycode as well as booleans to specify if there are
//  any meta keys pressed down as well.
// ----------------------------------------------------------------------------
class KeyPressInputEvent : public InputEvent {
public:
   const Key key;
   const bool alt;
   const bool ctrl;
   const bool shift;
   const bool system;

   KeyPressInputEvent(Key key, bool alt, bool ctrl, bool shift, bool system)
   : key(key)
   , alt(alt)
   , ctrl(ctrl)
   , shift(shift)
   , system(system)
   {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }
};

#endif
