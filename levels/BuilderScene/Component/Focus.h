#ifndef FOCUS_H
#define FOCUS_H

#include "Component.h"

// ----------------------------------------------------------------------------
// Focus
//
// This component lets you tag entities that have the user's "focus" (as in
// the GUI meaning of the word).
// ----------------------------------------------------------------------------
class Focus : public Component {
public:
   explicit Focus(const std::string& id = "Focus Component");
   Focus(const Focus& other);
   virtual ~Focus();

   Focus& operator=(const Focus& other);
   void swap(Focus& other);
};

#endif
