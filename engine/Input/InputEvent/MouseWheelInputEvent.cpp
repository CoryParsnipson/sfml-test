#include "MouseWheelInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const MouseWheelInputEvent& event) {
   stream << event.to_string();
   return stream;
}
