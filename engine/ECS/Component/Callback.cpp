#include "Callback.h"

Callback::Callback(const std::string& id /* = "Callback Component" */, bool propagate /* = true */)
: Component(id)
, propagate_(propagate)
, prev_mouse_wheel_pos_(0.f)
, prev_mouse_pos_(0, 0)
{
   // initialize all callback functions with null callbacks
   // TODO: find a better way to do this...
   this->callbacks_[CallbackKey::MOUSE_IN] = [] () {};
   this->callbacks_[CallbackKey::MOUSE_OUT] = [] () {};
   this->callbacks_[CallbackKey::MOUSE_MOVE] = [] () {};
   this->callbacks_[CallbackKey::MOUSE_WHEEL] = [] () {};
   this->callbacks_[CallbackKey::LEFT_CLICK] = [] () {};
   this->callbacks_[CallbackKey::RIGHT_CLICK] = [] () {};
   this->callbacks_[CallbackKey::LEFT_RELEASE] = [] () {};
   this->callbacks_[CallbackKey::RIGHT_RELEASE] = [] () {};
   this->callbacks_[CallbackKey::CAMERA_RESIZE] = [] () {};
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

float Callback::prev_mouse_wheel_pos() const {
   return this->prev_mouse_wheel_pos_;
}

void Callback::prev_mouse_wheel_pos(float pos) {
   this->prev_mouse_wheel_pos_ = pos;
}

sf::Vector2f Callback::prev_mouse_pos() const {
   return this->prev_mouse_pos_;
}

void Callback::prev_mouse_pos(sf::Vector2f pos) {
   this->prev_mouse_pos_ = pos;
}

bool Callback::propagate() const {
   return this->propagate_;
}

void Callback::propagate(bool propagate) {
   this->propagate_ = propagate;
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

void Callback::mouse_wheel(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::MOUSE_WHEEL] = callback;
}

void Callback::mouse_wheel() {
   this->callbacks_[CallbackKey::MOUSE_WHEEL]();
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

void Callback::camera_resize(Callback::CallbackT callback) {
   this->callbacks_[CallbackKey::CAMERA_RESIZE] = callback;
}

void Callback::camera_resize() {
   this->callbacks_[CallbackKey::CAMERA_RESIZE]();
}
