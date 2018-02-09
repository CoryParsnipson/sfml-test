#include "LostFocusInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const LostFocusInputEvent& event) {
   stream << event.to_string();
   return stream;
}
