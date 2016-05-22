#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <string>
#include <iostream>

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class InputListener;

// ----------------------------------------------------------------------------
// InputEvent abstract base
//
// This event is the base class for all input events. These are supposed to be
// collected by the Input object and passed to all input listeners.
// ----------------------------------------------------------------------------
class InputEvent
{
public:
   InputEvent(std::string id = "InputEvent") : id_(std::move(id)) {}
   virtual ~InputEvent() {}

   virtual void execute(InputListener& listener) = 0;

   operator std::string() const {
      return this->to_string();
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const InputEvent& event);

   std::string to_string() const {
      return "[InputEvent: " + this->id_ + "]";
   }

protected:
   std::string id_;
};

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
template <class T>
std::ostream& operator<<(std::ostream& stream, const InputEvent& event) {
   stream << event.to_string();
   return stream;
}

#endif
