#ifndef SYSTEM_H
#define SYSTEM_H

#include "Update.h"

class System
: public Update
{
public:
   System(const std::string& id = "System");
   virtual ~System();

   const std::string& id() const;

   virtual void init();
   virtual void update(Game& game) = 0;
   virtual void message();

private:
   std::string& id_;
};

#endif
