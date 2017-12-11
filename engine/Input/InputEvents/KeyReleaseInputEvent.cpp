#include "KeyReleaseInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const KeyReleaseInputEvent& event) {
   stream << event.to_string();
   return stream;
}
