#ifndef CLICKABLE_H
#define CLICKABLE_H

#include "Component.h"

// ----------------------------------------------------------------------------
// Clickable
//
// Make this component respond to mouse click and release events via the
// CallbackSystem.
// ----------------------------------------------------------------------------
class Clickable : public Component {
public:
   explicit Clickable(const std::string& id = "Clickable Component");
   Clickable(const Clickable& other);
   virtual ~Clickable();

   Clickable& operator=(const Clickable& other);
   void swap(Clickable& other);

   bool is_clicked() const;
   void is_clicked(bool is_clicked);
   
private:
   bool is_clicked_;
};

#endif
