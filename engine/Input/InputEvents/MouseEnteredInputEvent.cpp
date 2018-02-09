#include "MouseEnteredInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const MouseEnteredInputEvent& event) {
   stream << event.to_string();
   return stream;
}
