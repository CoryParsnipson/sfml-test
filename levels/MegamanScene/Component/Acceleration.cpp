#include "Acceleration.h"

Acceleration::Acceleration(const std::string& id /* = "Acceleration Component" */, float x /* = 0.f */, float y /* = 0.f */)
: Component(id)
, acceleration_(x, y)
{
}

Acceleration::Acceleration(const Acceleration& other)
: Component(other.id())
, acceleration_(other.acceleration_)
{
}

Acceleration::~Acceleration() {
}

Acceleration& Acceleration::operator=(const Acceleration& other) {
   Acceleration tmp(other);
   this->swap(tmp);
   return *this;
}

void Acceleration::swap(Acceleration& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->acceleration_, other.acceleration_);
}

void Acceleration::x(float x) {
   this->acceleration_.x = x;
}

float Acceleration::x() const {
   return this->acceleration_.x;
}

void Acceleration::y(float y) {
   this->acceleration_.y = y;
}

float Acceleration::y() const {
   return this->acceleration_.y;
}

void Acceleration::value(sf::Vector2f value) {
   this->acceleration_ = value;
}

const sf::Vector2f& Acceleration::value() const {
   return this->acceleration_;
}
