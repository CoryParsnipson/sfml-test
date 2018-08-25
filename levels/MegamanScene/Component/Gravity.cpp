#include <cmath>

#include "Gravity.h"

Gravity::Gravity(const std::string& id /* = "Gravity Component" */, sf::Vector2f gravity /* = sf::Vector2f(0, 1) */)
: Component(id)
, direction_(gravity)
{
}

Gravity::Gravity(const Gravity& other)
: Component(other.id())
, direction_(other.value())
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
}

void Gravity::value(sf::Vector2f gravity) {
   this->direction_ = gravity;
}

const sf::Vector2f& Gravity::value() const {
   return this->direction_;
}
