#include <cmath>

#include "Gravity.h"

Gravity::Gravity(const std::string& id /* = "Gravity Component" */, sf::Vector2f direction /* = sf::Vector2f(0, 1) */, float magnitude /* = 1.f */)
: Component(id)
, direction_(direction)
, magnitude_(magnitude)
{
}

Gravity::Gravity(const Gravity& other)
: Component(other.id())
, direction_(other.direction())
, magnitude_(other.magnitude())
{
}

Gravity::~Gravity() {
}

Gravity& Gravity::operator=(const Gravity& other) {
   Gravity tmp(other);
   this->swap(tmp);
   return *this;
}

void Gravity::swap(Gravity& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->direction_, other.direction_);
   std::swap(this->magnitude_, other.magnitude_);
}

void Gravity::direction(sf::Vector2f direction) {
   // normalize this into a unit vector
   float norm = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));

   direction.x = direction.x / norm;
   direction.y = direction.y / norm;

   this->direction_ = direction;
}

const sf::Vector2f& Gravity::direction() const {
   return this->direction_;
}

void Gravity::magnitude(float magnitude) {
   this->magnitude_ = magnitude;
}

float Gravity::magnitude() const {
   return this->magnitude_;
}
