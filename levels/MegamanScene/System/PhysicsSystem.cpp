#include <cmath>
#include <cassert>

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

#include <SFML/Graphics.hpp>

PhysicsSystem::PhysicsSystem(const std::string& id)
: System(id, new PreorderEntitySubscription(this, id + "EntitySubscription"))
, collision_time(1.f)
, normal()
, overlap()
, algorithm()
, scale_x(0.f)
, scale_y(0.f)
, sign_x(0)
, sign_y(0)
, near_time_x(0.f)
, near_time_y(0.f)
, far_time_x(0.f)
, far_time_y(0.f)
, delta_x(0.f)
, delta_y(0.f)
, overlap_x(0.f)
, overlap_y(0.f)
, dotprod(0.f)
, e_center()
, other_e_center()
, e_collision()
, other_e_collision()
, world_bounds_()
{
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Space, Collision, Velocity, Acceleration>();

   // initialize world bounds
   this->world_bounds_.left = 0;
   this->world_bounds_.top = 0;
   this->world_bounds_.width = game.window().size().x;
   this->world_bounds_.height = game.window().size().y;
}

void PhysicsSystem::on_update(Game& game, Entity& e) {
   // collect entities that could collide (not optimized)
   const std::vector<Handle>& physics_entities = this->subscription().entity_list();
   for (std::vector<Handle>::const_iterator other_it = physics_entities.begin(); other_it != physics_entities.end(); ++other_it) {
      Entity& other_e = *(this->scene().get_entity(*other_it));

      // don't collide with self
      if (&other_e == &e) {
         continue;
      }

      if (e.get<Velocity>()->value() == sf::Vector2f(0, 0) && other_e.get<Velocity>()->value() != sf::Vector2f(0, 0)) {
         // if this isn't moving and the other entity is, skip this (collision will be handled by other entity)
         continue;
      }

      this->e_center = this->global_transform(e).transformPoint(e.get<Collision>()->center());
      this->other_e_center = this->global_transform(other_e).transformPoint(other_e.get<Collision>()->center());

      this->e_collision = this->global_transform(e).transformRect(e.get<Collision>()->volume());
      this->other_e_collision = this->global_transform(other_e).transformRect(other_e.get<Collision>()->volume());

      this->e_collision.left -= this->global_transform(e).transformPoint(sf::Vector2f(0, 0)).x;
      this->e_collision.top -= this->global_transform(e).transformPoint(sf::Vector2f(0, 0)).y;

      this->other_e_collision.left -= this->global_transform(other_e).transformPoint(sf::Vector2f(0, 0)).x;
      this->other_e_collision.top -= this->global_transform(other_e).transformPoint(sf::Vector2f(0, 0)).y;

      this->delta_x = this->other_e_center.x - this->e_center.x;
      this->overlap_x = (this->other_e_collision.width / 2.f + this->e_collision.width / 2.f) - std::abs(this->delta_x);

      this->delta_y = this->other_e_center.y - this->e_center.y;
      this->overlap_y = (this->other_e_collision.height / 2.f + this->e_collision.height / 2.f) - std::abs(this->delta_y);

      if (e.get<Velocity>()->value() == sf::Vector2f(0, 0)) {
         this->algorithm = "AABB Intersection";

         // do AABB intersection (if not moving)
         if (this->overlap_x >= 0 && this->overlap_y >= 0) {
            this->collision_time = 0.f;

            if (this->overlap_x < this->overlap_y) {
               this->normal = sf::Vector2f(this->delta_x >= 0.f ? 1.f : 0.f, 0.f);
               this->overlap = sf::Vector2f(this->overlap_x, 0);
            } else {
               this->normal = sf::Vector2f(0.f, this->delta_y >= 0.f ? 1.f : 0.f);
               this->overlap = sf::Vector2f(0, this->overlap_y);
            }
         } else {
            this->collision_time = 1.f;
         }
      } else {
         this->algorithm = "Swept AABB";

         // perform sweptAABB (if moving)
         this->scale_x = e.get<Velocity>()->x() == 0.f ? 1.f : 1.f / e.get<Velocity>()->x();
         this->scale_y = e.get<Velocity>()->y() == 0.f ? 1.f : 1.f / e.get<Velocity>()->y();

         this->sign_x = (e.get<Velocity>()->x() >= 0 ? 1.f : -1.f);
         this->sign_y = (e.get<Velocity>()->y() >= 0 ? 1.f : -1.f);

         this->near_time_x = (
            e.get<Velocity>()->x() == 0.f
               ? -std::numeric_limits<float>::infinity()
               : (this->other_e_center.x - this->sign_x * (this->e_collision.width / 2.f + this->other_e_collision.width / 2.f) - this->e_center.x) * this->scale_x
         );

         this->near_time_y = (
            e.get<Velocity>()->y() == 0.f
               ? -std::numeric_limits<float>::infinity()
               : (this->other_e_center.y - this->sign_y * (this->e_collision.height / 2.f + this->other_e_collision.height / 2.f) - this->e_center.y) * this->scale_y
         );

         this->far_time_x = (
            e.get<Velocity>()->x() == 0.f
               ? std::numeric_limits<float>::infinity()
               : (this->other_e_center.x + this->sign_x * (this->e_collision.width / 2.f + this->other_e_collision.width / 2.f) - this->e_center.x) * this->scale_x
         );

         this->far_time_y = (
            e.get<Velocity>()->y() == 0.f
               ? std::numeric_limits<float>::infinity()
               : (this->other_e_center.y + this->sign_y * (this->e_collision.height / 2.f + this->other_e_collision.height / 2.f) - this->e_center.y) * this->scale_y
         );

         float near_time = std::max(this->near_time_x, this->near_time_y);
         float far_time = std::min(this->far_time_x, this->far_time_y);

         if (((this->near_time_x > this->far_time_y || this->near_time_y > this->far_time_x)
             || near_time >= 1
             || far_time <= 0.f)
             || (this->near_time_x == -std::numeric_limits<float>::infinity() && this->overlap_x <= 0.f) // these two conditions solve "ghost collisions" on each axis
             || (this->near_time_y == -std::numeric_limits<float>::infinity() && this->overlap_y <= 0.f) // (these aren't on the algorithm references. If you use broad phase culling, you can allegedly remove these conditions)
         ) {
            this->collision_time = 1.f;
         } else {
            if (this->near_time_x > this->near_time_y) {
               this->normal = sf::Vector2f(-1.f * this->sign_x, 0.f);
            } else {
               this->normal = sf::Vector2f(0.f, -1.f * this->sign_y);
            }

            this->collision_time = near_time;
            this->overlap = sf::Vector2f(e.get<Velocity>()->x() * this->scale_x, e.get<Velocity>()->y() * this->scale_y);
         }
      }

      // collision response
      sf::Vector2f position_delta;

      // 1. move entity to scaled velocity
      position_delta.x = e.get<Velocity>()->x() * this->collision_time;
      position_delta.y = e.get<Velocity>()->y() * this->collision_time;

      float remaining_time = 1.f - this->collision_time;

      // 2. slide the rest of the way
      if (remaining_time > 0.f) {
         this->dotprod = (e.get<Velocity>()->x() * this->normal.y + e.get<Velocity>()->y() * this->normal.x) * remaining_time;

         position_delta.x += this->dotprod * this->normal.y;
         position_delta.y += this->dotprod * this->normal.x;

         if (this->normal.x != 0.f) {
            e.get<Velocity>()->x(0);
            e.get<Acceleration>()->x(0);
         }

         if (this->normal.y != 0.f) {
            e.get<Velocity>()->y(0);
            e.get<Acceleration>()->y(0);
         }
      }

      // update position based on velocity
      e.get<Space>()->move(position_delta);
      // end collision response
   }

   if (this->collision_time == 1.f) {
      // accelerate if there's no collision
      sf::Vector2f total_acceleration = e.get<Acceleration>()->value();

      // add gravity to accleration
      if (e.get<Gravity>() != nullptr) {
         total_acceleration += e.get<Gravity>()->value();
      }
      e.get<Velocity>()->value(e.get<Velocity>()->value() + total_acceleration);
   }

   this->clamp_entity(e);
}

void PhysicsSystem::clamp_entity(Entity& e) {
   // this will make things easier for now
   sf::Vector2f entity_pos = this->global_transform(e).transformPoint(0, 0);
   sf::FloatRect world_bounds = this->world_bounds_;

   sf::FloatRect collision = this->global_transform(e).transformRect(e.get<Collision>()->volume());

   world_bounds.width -= collision.width;
   world_bounds.height -= collision.height;

   if (!world_bounds.contains(entity_pos)) {
      bool was_clamped_x = false;
      bool was_clamped_y = false;
      sf::Vector2f final_pos = entity_pos;

      final_pos.x = this->clamp(entity_pos.x, world_bounds.left, world_bounds.left + world_bounds.width, was_clamped_x);
      if (was_clamped_x) {
         e.get<Velocity>()->x(0);
         e.get<Acceleration>()->x(0);
      }

      final_pos.y = this->clamp(entity_pos.y, world_bounds.top, world_bounds.top + world_bounds.height, was_clamped_y);
      if (was_clamped_y) {
         e.get<Velocity>()->y(0);
         e.get<Acceleration>()->y(0);
      }

      e.get<Space>()->position(final_pos);
   }
}

float PhysicsSystem::clamp(float value, float min, float max, bool& was_clamped) {
   was_clamped = false;

   if (value < min) {
      was_clamped = true;
      return min;
   } else if (value > max) {
      was_clamped = true;
      return max;
   }

   return value;
}
