#include "MouseButtonInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const MouseButtonInputEvent& event) {
   stream << event.to_string();
   return stream;
}
