#include "InputEvent.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const InputEvent& event) {
   stream << event.to_string();
   return stream;
}
