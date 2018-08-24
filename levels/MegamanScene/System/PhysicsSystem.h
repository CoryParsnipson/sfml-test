#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "System.h"

class PhysicsSystem : public System {
public:
   PhysicsSystem(const std::string& id);
   virtual ~PhysicsSystem();

private:
   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
