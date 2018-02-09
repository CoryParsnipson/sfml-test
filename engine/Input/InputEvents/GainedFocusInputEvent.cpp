#include "GainedFocusInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const GainedFocusInputEvent& event) {
   stream << event.to_string();
   return stream;
}
