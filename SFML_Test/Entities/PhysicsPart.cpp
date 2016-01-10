#include "PhysicsPart.h"

PhysicsPart::PhysicsPart(std::string id, sf::FloatRect bounding_box)
: Part(id)
, solid_(false)
, bounding_box_(bounding_box)
{
   Service::get_logger().msg("PhysicsPart", Logger::INFO, "Creating PhysicsPart '" + id + "'");
}

PhysicsPart::~PhysicsPart() {
}

void PhysicsPart::set_position(sf::Vector2f pos) {
   this->bounding_box_.left = pos.x;
   this->bounding_box_.top = pos.y;
}

void PhysicsPart::set_position(float x, float y) {
   this->bounding_box_.left = x;
   this->bounding_box_.top = y;
}

sf::Vector2f PhysicsPart::get_position() {
   return sf::Vector2f(this->bounding_box_.left, this->bounding_box_.top);
}

void PhysicsPart::set_solid(bool solid) {
   this->solid_ = solid;
}

bool PhysicsPart::is_solid() {
   return this->solid_;
}

bool PhysicsPart::intersects(sf::Vector2i other_point) {
   return this->bounding_box_.contains(static_cast<sf::Vector2f>(other_point));
}

bool PhysicsPart::intersects(sf::Vector2f other_point) {
   return this->bounding_box_.contains(other_point);
}

bool PhysicsPart::intersects(sf::FloatRect other_bounding_box) {
   return this->bounding_box_.intersects(other_bounding_box);
}

void PhysicsPart::update(Entity& entity, Game& game) {
}
