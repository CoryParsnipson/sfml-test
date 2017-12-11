#ifndef KEY_RELEASE_INPUT_EVENT_H
#define KEY_RELEASE_INPUT_EVENT_H

#include "InputManager.h"
#include "InputEvent.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
//  KeyReleaseInputEvent
//
//  This event should be emitted when the user releases a key on the keyboard.
//  The event contains a keycode as well as booleans to specify if there are
//  any meta keys released as well.
// ----------------------------------------------------------------------------
class KeyReleaseInputEvent : public InputEvent {
public:
   const Key key;
   const bool alt;
   const bool ctrl;
   const bool shift;
   const bool system;

   KeyReleaseInputEvent(Key key, bool alt, bool ctrl, bool shift, bool system)
   : InputEvent("KeyRelease")
   , key(key)
   , alt(alt)
   , ctrl(ctrl)
   , shift(shift)
   , system(system)
   {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const KeyReleaseInputEvent& event);
};

#endif
