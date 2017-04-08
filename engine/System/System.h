#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>

#include "Update.h"

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

private:
   std::string id_;
};

#endif
