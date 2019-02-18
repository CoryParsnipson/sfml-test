#include "Clickable.h"

Clickable::Clickable(const std::string& id /* = "Clickable Component" */, bool is_enabled /* = true */)
: Component(id)
, is_left_clicked_(false)
, is_right_clicked_(false)
, is_enabled_(is_enabled)
{
}

Clickable::Clickable(const Clickable& other)
: Component(other.id())
, is_left_clicked_(other.is_left_clicked())
, is_right_clicked_(other.is_left_clicked())
, is_enabled_(other.is_enabled_)
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
   std::swap(this->is_enabled_, other.is_enabled_);
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

bool Clickable::is_enabled() const {
   return this->is_enabled_;
}

void Clickable::is_enabled(bool is_enabled) {
   this->is_enabled_ = is_enabled;
}

const sf::Vector2f& Clickable::left_click_pos() const {
   return this->left_click_pos_;
}

void Clickable::left_click_pos(sf::Vector2f pos) {
   this->left_click_pos_ = pos;
}

const sf::Vector2f& Clickable::right_click_pos() const {
   return this->right_click_pos_;
}

void Clickable::right_click_pos(sf::Vector2f pos) {
   this->right_click_pos_ = pos;
}
