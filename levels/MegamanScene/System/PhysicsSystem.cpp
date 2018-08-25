#include <cmath>

#include "PhysicsSystem.h"
#include "PreorderEntitySubscription.h"

#include "Game.h"
#include "Scene.h"
#include "Entity.h"

#include "Space.h"
#include "Collision.h"
#include "Velocity.h"
#include "Acceleration.h"
#include "Gravity.h"

PhysicsSystem::PhysicsSystem(const std::string& id)
: System(id, new PreorderEntitySubscription(this, id + "EntitySubscription"))
{
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Space, Collision, Velocity, Acceleration>();
}

void PhysicsSystem::on_update(Game& game, Entity& e) {
   bool collision_on_x = false;
   bool collision_on_y = false;

   // perform collision detection (not optimized)
   std::vector<Handle> physics_entities;
   std::vector<Handle> entities = this->scene().entities();
   for (std::vector<Handle>::iterator it = entities.begin(); it != entities.end(); ++it) {
      Entity* e = this->scene().get_entity(*it);

      if (this->subscribe_to().filter(*e)) {
         physics_entities.push_back(*it);
      }
   }

   for (std::vector<Handle>::iterator it = physics_entities.begin(); it != physics_entities.end() - 1; ++it) {
      for (std::vector<Handle>::iterator other_it = it + 1; other_it != physics_entities.end(); ++other_it) {
         Entity* e = this->scene().get_entity(*it);
         Entity* other_e = this->scene().get_entity(*other_it);

         sf::FloatRect e_collision = this->global_transform(*e).transformRect(e->get<Collision>()->volume());
         sf::FloatRect other_e_collision = this->global_transform(*other_e).transformRect(other_e->get<Collision>()->volume());

         if (!e_collision.intersects(other_e_collision)) {
            continue;
         }

         float overlap_x = std::abs(e_collision.left - (other_e_collision.left + other_e_collision.width)) - (e_collision.width + other_e_collision.width);
         float overlap_y = std::abs(e_collision.top - (other_e_collision.top + other_e_collision.height)) - (e_collision.height + other_e_collision.height);

         // find out intersection in each dimension and set velocity to zero in those directions
         if (overlap_x < 0) {
            collision_on_x = true;
         }

         if (overlap_y < 0) {
            collision_on_y = true;
         }
      }
   }

   sf::Vector2f total_acceleration = e.get<Acceleration>()->value();

   // add gravity to accleration
   if (e.get<Gravity>() != nullptr) {
      total_acceleration += e.get<Gravity>()->value();
   }
   
   // update velocity based on acceleration
   e.get<Velocity>()->value(e.get<Velocity>()->value() + total_acceleration);

   if (collision_on_x) {
      e.get<Velocity>()->x(0);
   }

   if (collision_on_y) {
      e.get<Velocity>()->y(0);
   }

   // update position based on velocity
   e.get<Space>()->move(e.get<Velocity>()->value());
}
