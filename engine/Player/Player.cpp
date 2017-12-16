#include "Player.h"

Player::Player(Player::PlayerId id, const std::string& name /* = "Player" */)
: id_(id)
, name_(name)
, binding_(name + "InputBinding")
{
}

Player::~Player() {
}

Player::PlayerId Player::id() const {
   return this->id_;
}

void Player::id(Player::PlayerId id) {
   this->id_ = id;
}

std::string Player::name() const {
   return this->name_;
}

void Player::name(const std::string& name) {
   this->name_ = name;
}

InputBinding& Player::bindings() {
   return this->binding_;
}
