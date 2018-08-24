#include "PhysicsSystem.h"
#include "PreorderEntitySubscription.h"

#include "Game.h"
#include "Entity.h"

#include "Space.h"
#include "Velocity.h"

PhysicsSystem::PhysicsSystem(const std::string& id)
: System(id, new PreorderEntitySubscription(this, id + "EntitySubscription"))
{
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Space, Velocity>();
}

void PhysicsSystem::on_update(Game& game, Entity& e) {
   e.get<Space>()->move(e.get<Velocity>()->value());
}
