#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>

#include "Update.h"
#include "ObjectPool.h"
#include "EntityFilter.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Game;
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
   System(const std::string& id = "System");
   virtual ~System();

   void id(const std::string& id);
   const std::string& id() const;

   EntityFilter& filter(); // get a reference to entity filter itself 
   bool filter(Entity& e) const; // use the entity filter to filter an entity

   void enable();
   void disable();

   bool is_enabled() const;

   void init(Game& game);
   void update(Game& game);
   void message();

   virtual void accept(SystemVisitor& visitor) = 0;

private:
   std::string id_;
   bool enabled_;
   EntityFilter filter_;
   std::vector<Handle> entities_;

   // System interface hooks
   virtual void pre_init(Game& game) {}
   virtual void on_init(Game& game) = 0;
   virtual void post_init(Game& game) {}

   virtual void pre_update(Game& game) {}
   virtual void on_update(Game& game, Entity& e) = 0;
   virtual void post_update(Game& game) {}

   //virtual void message() = 0; // TODO: implement stub
};

#endif
