#include "MouseMoveInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const MouseMoveInputEvent& event) {
   stream << event.to_string();
   return stream;
}
