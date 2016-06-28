#include "CloseInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const CloseInputEvent& event) {
   stream << event.to_string();
   return stream;
}
