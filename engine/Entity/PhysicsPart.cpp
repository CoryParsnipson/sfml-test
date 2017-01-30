#include "PhysicsPart.h"
#include "Game.h"

PhysicsPart::PhysicsPart(std::string id, sf::FloatRect bounding_box)
: Part(id)
, solid_(false)
, bounding_box_(bounding_box)
{
   Game::logger().msg("PhysicsPart", Logger::INFO, "Creating PhysicsPart '" + id + "'");
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

bool PhysicsPart::intersects(const sf::Vector2i& other) {
   return this->bounding_box_.contains(static_cast<sf::Vector2f>(other));
}

bool PhysicsPart::intersects(const sf::Vector2f& other) {
   return this->bounding_box_.contains(other);
}

bool PhysicsPart::intersects(const sf::FloatRect& other) {
   return this->bounding_box_.intersects(other);
}

void PhysicsPart::update(Game& game, Scene* scene) {
}

std::string PhysicsPart::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["x"] = std::to_string(this->bounding_box_.left);
   data["y"] = std::to_string(this->bounding_box_.top);

   data["width"] = std::to_string(this->bounding_box_.width);
   data["height"] = std::to_string(this->bounding_box_.height);

   return s.serialize(data);
}

void PhysicsPart::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   this->set_position(sf::Vector2f(std::stod(data["x"]), std::stod(data["y"])));
   this->set_size(sf::Vector2f(std::stod(data["width"]), std::stod(data["height"])));
}
