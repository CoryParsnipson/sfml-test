#include "Collision.h"

Collision::Collision(const std::string& id /* = "Collision Component" */, sf::FloatRect volume /* = sf::FloatRect(0, 0, 0, 0) */)
: Component(id)
, volume_(volume)
{
}

Collision::Collision(const Collision& other)
: Component(other.id())
{
}

Collision::~Collision() {
}

Collision& Collision::operator=(const Collision& other) {
   Collision tmp(other);
   this->swap(tmp);
   return *this;
}

void Collision::swap(Collision& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->volume_, other.volume_);
}

bool Collision::contains(sf::Vector2f point) const {
   return this->volume_.contains(point);
}

bool Collision::intersects(sf::FloatRect other) const {
   return this->volume_.intersects(other);
}

void Collision::volume(sf::FloatRect volume) {
   this->volume_ = volume;
}

void Collision::volume(sf::Vector2f pos, sf::Vector2f size) {
   this->volume_.left = pos.x;
   this->volume_.top = pos.y;
   this->volume_.width = size.x;
   this->volume_.height = size.y;
}

const sf::FloatRect& Collision::volume() const {
   return this->volume_;
}

sf::Vector2f Collision::center() {
   sf::Vector2f center(this->volume_.left + this->volume_.width / 2.f, this->volume_.top + this->volume_.height / 2.f);
   return center;
}
