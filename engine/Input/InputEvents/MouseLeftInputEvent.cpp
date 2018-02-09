#include "MouseLeftInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const MouseLeftInputEvent& event) {
   stream << event.to_string();
   return stream;
}
