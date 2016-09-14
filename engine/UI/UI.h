#ifndef UI_H
#define UI_H

namespace UI {
   // -------------------------------------------------------------------------
   // UI common enum classes
   // -------------------------------------------------------------------------
   enum class Positioning {
      Default,    // position children widget relative to the parent widget
      Absolute,   // position children relative to the parent widget's scene graph node
   };
}
#endif
