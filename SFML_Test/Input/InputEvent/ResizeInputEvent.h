#ifndef RESIZE_INPUT_EVENT_H
#define RESIZE_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// ResizeInputEvent
//
// This event should be emitted when the game window has been resized. It will
// be emitted each game loop iteration while the user is clicking and dragging
// the game window.
// ----------------------------------------------------------------------------
class ResizeInputEvent : public InputEvent {
public:
   const int width;
   const int height;

   ResizeInputEvent(int width, int height)
   : width(width)
   , height(height)
   {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }
};

#endif
