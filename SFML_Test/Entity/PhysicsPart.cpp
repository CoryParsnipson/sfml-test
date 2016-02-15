#include "PhysicsPart.h"

#include "Viewport.h"

PhysicsPart::PhysicsPart(std::string id, sf::FloatRect bounding_box)
: Part(id)
, solid_(false)
, bounding_box_(bounding_box)
{
   Service::get_logger().msg("PhysicsPart", Logger::INFO, "Creating PhysicsPart '" + id + "'");
}

PhysicsPart::~PhysicsPart() {
}

void PhysicsPart::set_position(float x, float y) {
   this->bounding_box_.left = x;
   this->bounding_box_.top = y;
}

void PhysicsPart::set_position(const sf::Vector2f& pos) {
   this->bounding_box_.left = pos.x;
   this->bounding_box_.top = pos.y;
}

sf::Vector2f PhysicsPart::get_position() {
   return sf::Vector2f(this->bounding_box_.left, this->bounding_box_.top);
}

void PhysicsPart::set_size(float width, float height) {
   this->bounding_box_.width = std::max(0.f, width);
   this->bounding_box_.height = std::max(0.f, height);
}

void PhysicsPart::set_size(const sf::Vector2f& size) {
   this->set_size(size.x, size.y);
}

sf::Vector2f PhysicsPart::get_size() {
   return sf::Vector2f(this->bounding_box_.width, this->bounding_box_.height);
}

sf::FloatRect PhysicsPart::get_bounding_box() {
   return this->bounding_box_;
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

void PhysicsPart::update(Game& game, Scene* scene, Entity* entity) {
}
