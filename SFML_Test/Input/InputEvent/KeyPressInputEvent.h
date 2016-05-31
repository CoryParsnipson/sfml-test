#ifndef KEY_PRESS_INPUT_EVENT_H
#define KEY_PRESS_INPUT_EVENT_H

#include "Input.h"
#include "InputEvent.h"
#include "InputListener.h"

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
