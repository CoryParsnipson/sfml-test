#include "Clickable.h"

Clickable::Clickable(const std::string& id /* = "Clickable Component" */)
: Component(id)
, is_clicked_(false)
{
}

Clickable::Clickable(const Clickable& other)
: Component(other.id())
, is_clicked_(other.is_clicked())
{
}

Clickable::~Clickable() {
}

Clickable& Clickable::operator=(const Clickable& other) {
   Clickable tmp(other);
   this->swap(tmp);
   return *this;
}

void Clickable::swap(Clickable& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->is_clicked_, other.is_clicked_);
}

bool Clickable::is_clicked() const {
   return this->is_clicked_;
}

void Clickable::is_clicked(bool is_clicked) {
   this->is_clicked_ = is_clicked;
}
