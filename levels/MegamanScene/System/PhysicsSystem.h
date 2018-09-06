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

   void clamp_entity(Entity& e);
   float clamp(float value, float min, float max, bool& was_clamped);

   sf::FloatRect world_bounds_;
};

#endif
