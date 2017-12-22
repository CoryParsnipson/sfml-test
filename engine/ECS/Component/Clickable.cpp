#include "Clickable.h"

Clickable::Clickable(const std::string& id /* = "Clickable Component" */)
: Component(id)
, is_left_clicked_(false)
, is_right_clicked_(false)
{
}

Clickable::Clickable(const Clickable& other)
: Component(other.id())
, is_left_clicked_(other.is_left_clicked())
, is_right_clicked_(other.is_left_clicked())
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
   std::swap(this->is_left_clicked_, other.is_left_clicked_);
   std::swap(this->is_right_clicked_, other.is_right_clicked_);
}

bool Clickable::is_left_clicked() const {
   return this->is_left_clicked_;
}

bool Clickable::is_right_clicked() const {
   return this->is_right_clicked_;
}

void Clickable::is_left_clicked(bool is_clicked) {
   this->is_left_clicked_ = is_clicked;
}

void Clickable::is_right_clicked(bool is_clicked) {
   this->is_right_clicked_ = is_clicked;
}
