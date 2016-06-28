#include "KeyPressInputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const KeyPressInputEvent& event) {
   stream << event.to_string();
   return stream;
}
