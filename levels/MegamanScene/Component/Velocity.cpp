#include "Velocity.h"

Velocity::Velocity(const std::string& id /* = "Velocity Component" */, float x /* = 0.f */, float y /* = 0.f */)
: Component(id)
, velocity_(x, y)
{
}

Velocity::Velocity(const Velocity& other)
: Component(other.id())
, velocity_(other.velocity_)
{
}

Velocity::~Velocity() {
}

Velocity& Velocity::operator=(const Velocity& other) {
   Velocity tmp(other);
   this->swap(tmp);
   return *this;
}

void Velocity::swap(Velocity& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->velocity_, other.velocity_);
}

void Velocity::x(float x) {
   this->velocity_.x = x;
}

float Velocity::x() const {
   return this->velocity_.x;
}

void Velocity::y(float y) {
   this->velocity_.y = y;
}

float Velocity::y() const {
   return this->velocity_.y;
}

void Velocity::value(sf::Vector2f value) {
   this->velocity_ = value;
}

const sf::Vector2f& Velocity::value() const {
   return this->velocity_;
}
