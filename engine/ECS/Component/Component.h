#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include "Serializable.h"

// ----------------------------------------------------------------------------
// Component
//
// This is the abstract base class for Components that can be added to an
// Entity.
// ----------------------------------------------------------------------------
class Component {
public:
   explicit Component(const std::string& id = "Component") : id_(id) {}
   Component(const Component& other) : id_(other.id_) {}
   virtual ~Component() {}

   Component& operator=(const Component& other) {
      Component tmp(other);
      this->swap(tmp);
      return *this;
   }

   void swap(Component& other) {
      std::swap(this->id_, other.id_);
   }

   const std::string& id() const {
      return this->id_;
   }

   void id(std::string id) {
      this->id_ = id;
   }

   // serialize interface
   virtual std::string serialize(Serializer& s) { return ""; }
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d) {}

private:
   std::string id_;
};

#endif
