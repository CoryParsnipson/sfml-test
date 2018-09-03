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

PhysicsSystem::PhysicsSystem(const std::string& id)
: System(id, new PreorderEntitySubscription(this, id + "EntitySubscription"))
, world_bounds_()
{
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Space, Collision, Velocity, Acceleration>();

   // initialize world bounds
   this->world_bounds_.left = -100;
   this->world_bounds_.top = -100;
   this->world_bounds_.width = game.window().size().x + 200;
   this->world_bounds_.height = game.window().size().y + 200;
}

void PhysicsSystem::on_update(Game& game, Entity& e) {
   sf::Vector2f overlap;

   // collect entities that could collide (not optimized)
   std::vector<Handle> physics_entities;
   std::vector<Handle> entities = this->scene().entities();
   for (std::vector<Handle>::iterator it = entities.begin(); it != entities.end(); ++it) {
      Entity* other_e = this->scene().get_entity(*it);
      if (other_e == &e) {
         continue;
      }

      if (this->subscribe_to().filter(*other_e)) {
         physics_entities.push_back(*it);
      }
   }

   for (std::vector<Handle>::iterator other_it = physics_entities.begin(); other_it != physics_entities.end(); ++other_it) {
      Entity& other_e = *(this->scene().get_entity(*other_it));

      //if (e.get<Velocity>()->value() == sf::Vector2f(0, 0) && other_e.get<Velocity>()->value() == sf::Vector2f(0, 0)) {
         // do AABB intersection (if not moving)
         sf::Vector2f e_center(
            e.get<Collision>()->volume().left + e.get<Collision>()->volume().width / 2.f,
            e.get<Collision>()->volume().top + e.get<Collision>()->volume().height / 2.f
         );

         sf::Vector2f other_e_center(
            other_e.get<Collision>()->volume().left + other_e.get<Collision>()->volume().width / 2.f,
            other_e.get<Collision>()->volume().top + other_e.get<Collision>()->volume().height / 2.f
         );

         e_center = this->global_transform(e).transformPoint(e_center);
         other_e_center = this->global_transform(other_e).transformPoint(other_e_center);

         sf::FloatRect e_collision = this->global_transform(e).transformRect(e.get<Collision>()->volume());
         sf::FloatRect other_e_collision = this->global_transform(other_e).transformRect(other_e.get<Collision>()->volume());

         e_collision.left -= this->global_transform(e).transformPoint(sf::Vector2f(0, 0)).x;
         e_collision.top -= this->global_transform(e).transformPoint(sf::Vector2f(0, 0)).y;

         other_e_collision.left -= this->global_transform(other_e).transformPoint(sf::Vector2f(0, 0)).x;
         other_e_collision.top -= this->global_transform(other_e).transformPoint(sf::Vector2f(0, 0)).y;

         float delta_x = other_e_center.x - e_center.x;
         float overlap_x = (other_e_collision.width / 2.f + e_collision.width / 2.f) - std::abs(delta_x);

         float delta_y = other_e_center.y - e_center.y;
         float overlap_y = (other_e_collision.height / 2.f + e_collision.height / 2.f) - std::abs(delta_y);

         overlap = sf::Vector2f(overlap_x, overlap_y);
      //} else {
      //   // perform sweptAABB (if moving)
      //}
   }

   sf::Vector2f total_acceleration = e.get<Acceleration>()->value();

   // add gravity to accleration
   if (e.get<Gravity>() != nullptr) {
      total_acceleration += e.get<Gravity>()->value();
   }

   sf::Vector2f old_velocity = e.get<Velocity>()->value();
   
   // update velocity based on acceleration
   e.get<Velocity>()->value(e.get<Velocity>()->value() + total_acceleration);

   // TODO: this might be AABB specific?
   // collision response (change velocity and back out box)
   if (overlap.x >= 0 && overlap.y >= 0 && old_velocity != sf::Vector2f(0, 0)) {
      if (overlap.x < overlap.y) {
         e.get<Velocity>()->x(0);
         e.get<Space>()->move(sf::Vector2f(-1 * overlap.x, 0));
      } else {
         e.get<Velocity>()->y(0);
         e.get<Space>()->move(sf::Vector2f(0, -1 * overlap.y));
      }
   }

   // update position based on velocity
   e.get<Space>()->move(e.get<Velocity>()->value());

   // this will make things easier for now
   sf::Vector2f final_pos = e.get<Space>()->position();
   if (!this->world_bounds_.contains(final_pos)) {
      if (final_pos.x < this->world_bounds_.left) {
         e.get<Space>()->position(sf::Vector2f(this->world_bounds_.left, final_pos.y));
         e.get<Velocity>()->x(0);
         e.get<Acceleration>()->x(0);
      } else if (final_pos.x > (this->world_bounds_.width + this->world_bounds_.left)) {
         e.get<Space>()->position(sf::Vector2f(this->world_bounds_.left + this->world_bounds_.width, final_pos.y));
         e.get<Velocity>()->x(0);
         e.get<Acceleration>()->x(0);
      }

      if (final_pos.y < this->world_bounds_.top) {
         e.get<Space>()->position(sf::Vector2f(final_pos.x, this->world_bounds_.top));
         e.get<Velocity>()->y(0);
         e.get<Acceleration>()->y(0);
      } else if (final_pos.y > (this->world_bounds_.top + this->world_bounds_.height)) {
         e.get<Space>()->position(sf::Vector2f(final_pos.x, this->world_bounds_.top + this->world_bounds_.height));
         e.get<Velocity>()->y(0);
         e.get<Acceleration>()->y(0);
      }
   }
}
