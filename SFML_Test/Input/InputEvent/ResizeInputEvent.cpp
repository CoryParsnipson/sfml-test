#include "ResizeInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const ResizeInputEvent& event) {
   stream << event.to_string();
   return stream;
}
