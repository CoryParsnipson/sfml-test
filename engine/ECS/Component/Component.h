#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

// ----------------------------------------------------------------------------
// Component
//
// This is the abstract base class for Components that can be added to an
// Entity.
// ----------------------------------------------------------------------------
class Component {
public:
   explicit Component(const std::string& id = "Component")
   : id_(id)
   {
   }

   virtual ~Component() {}

   const std::string& id() const {
      return this->id_;
   }

   void id(std::string id) {
      this->id_ = id;
   }

private:
   std::string id_;
};

#endif
