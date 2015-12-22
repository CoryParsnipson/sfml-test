#include "PhysicsPart.h"

PhysicsPart::PhysicsPart(std::string id)
: Part(id)
, pos(0, 0)
{
   Service::get_logger().msg("PhysicsPart", Logger::INFO, "Creating PhysicsPart '" + id + "'");
}

PhysicsPart::~PhysicsPart() {
}

void PhysicsPart::set_position(sf::Vector2f pos) {
   this->pos = pos;
}

void PhysicsPart::set_position(float x, float y) {
   this->pos.x = x;
   this->pos.y = y;
}

sf::Vector2f PhysicsPart::get_position() {
   return this->pos;
}

void PhysicsPart::update(Entity2& entity, Viewport& viewport) {
}
