#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "System.h"

class PhysicsSystem : public System {
public:
   PhysicsSystem(const std::string& id);
   virtual ~PhysicsSystem();

   // public for debug
   float collision_time;
   sf::Vector2f normal;
   sf::Vector2f overlap;

   std::string algorithm;

   float scale_x;
   float scale_y;

   int sign_x;
   int sign_y;

   float near_time_x;
   float near_time_y;

   float far_time_x;
   float far_time_y;

   float delta_x;
   float delta_y;

   float overlap_x;
   float overlap_y;

   float dotprod;

   sf::Vector2f e_center;
   sf::Vector2f other_e_center;

   sf::FloatRect e_collision;
   sf::FloatRect other_e_collision;

private:
   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);

   void clamp_entity(Entity& e);
   float clamp(float value, float min, float max, bool& was_clamped);

   sf::FloatRect world_bounds_;
};

#endif
