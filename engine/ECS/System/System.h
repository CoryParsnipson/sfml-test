#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>

#include "Update.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class SystemVisitor;

// ----------------------------------------------------------------------------
// System
//
// This is the base class for anything that needs to operate on and/or
// between Entities and Components.
// ----------------------------------------------------------------------------
class System
: public Update
{
public:
   System(const std::string& id = "System")
   : id_(id)
   {
   }

   virtual ~System() {}

   const std::string& id() const { return this->id_; }

   virtual void init() {}
   virtual void update(Game& game) = 0;
   virtual void message() {}

   // visitor interface
   virtual void accept(SystemVisitor& visitor) = 0;

private:
   std::string id_;
   std::vector<Handle> subscribed_entities_;
};

#endif
