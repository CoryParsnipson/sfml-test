#include "Callback.h"

Callback::Callback(const std::string& id /* = "Callback Component" */)
: Component(id)
{
}

Callback::Callback(const Callback& other)
: Component(other.id())
, callbacks_(other.callbacks_)
{
}

Callback::~Callback() {
   this->callbacks_.clear();
}

Callback& Callback::operator=(const Callback& other) {
   Callback tmp(other);
   this->swap(tmp);
   return *this;
}

void Callback::swap(Callback& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->callbacks_, other.callbacks_);
}

sf::Vector2f Callback::prev_mouse_pos() const {
   return this->prev_mouse_pos_;
}

void Callback::prev_mouse_pos(sf::Vector2f pos) {
   this->prev_mouse_pos_ = pos;
}

void Callback::mouse_in(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::MOUSE_IN] = callback;
}

void Callback::mouse_in() {
   this->callbacks_[CallbackKey::MOUSE_IN]();
}

void Callback::mouse_out(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::MOUSE_OUT] = callback;
}

void Callback::mouse_out() {
   this->callbacks_[CallbackKey::MOUSE_OUT]();
}

void Callback::mouse_move(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::MOUSE_MOVE] = callback;
}

void Callback::mouse_move() {
   this->callbacks_[CallbackKey::MOUSE_MOVE]();
}

void Callback::left_click(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::LEFT_CLICK] = callback;
}

void Callback::left_click() {
   this->callbacks_[CallbackKey::LEFT_CLICK]();
}

void Callback::right_click(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::RIGHT_CLICK] = callback;
}

void Callback::right_click() {
   this->callbacks_[CallbackKey::RIGHT_CLICK]();
}

void Callback::left_release(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::LEFT_RELEASE] = callback;
}

void Callback::left_release() {
   this->callbacks_[CallbackKey::LEFT_RELEASE]();
}

void Callback::right_release(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::RIGHT_RELEASE] = callback;
}

void Callback::right_release() {
   this->callbacks_[CallbackKey::RIGHT_RELEASE]();
}
