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
   : InputEvent("Resize")
   , width(width)
   , height(height)
   {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const ResizeInputEvent& event);

protected:
   virtual std::string to_string() const {
      return "[InputEvent: " + this->id_ + " (" + std::to_string(this->width) + ", " + std::to_string(this->height) + "]";
   }
};

#endif
