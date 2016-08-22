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

Serialize::SerialObj PhysicsPart::serialize() {
   Serialize::SerialObj obj;

   // no need to include type tag in Part serialization
   obj["pos_x"] = std::to_string(this->bounding_box_.left);
   obj["pos_y"] = std::to_string(this->bounding_box_.top);

   obj["bounds_width"] = std::to_string(this->bounding_box_.width);
   obj["bounds_height"] = std::to_string(this->bounding_box_.height);

   return obj;
}

void PhysicsPart::deserialize(Serialize::SerialObj& obj, const TextureAtlas* textures /* = nullptr */) {
   sf::Vector2f pos;
   sf::Vector2f size;

   pos.x = std::stod(obj["pos_x"]);
   pos.y = std::stod(obj["pos_y"]);

   size.x = std::stod(obj["bounds_width"]);
   size.y = std::stod(obj["bounds_height"]);

   this->set_position(pos);
   this->set_size(size);
}
