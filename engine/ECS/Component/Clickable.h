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

   bool is_left_clicked() const;
   void is_left_clicked(bool is_clicked);

   bool is_right_clicked() const;
   void is_right_clicked(bool is_clicked);
   
private:
   bool is_left_clicked_;
   bool is_right_clicked_;
};

#endif
