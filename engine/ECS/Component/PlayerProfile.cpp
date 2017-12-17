#include "PlayerProfile.h"

PlayerProfile::PlayerProfile(const std::string& id /* = "PlayerProfile Component" */, Player::PlayerId player_id /* = 0 */)
: Component(id)
, player_id_(player_id)
{
}

PlayerProfile::PlayerProfile(const PlayerProfile& other)
: Component(other.id())
, player_id_(other.player_id_)
{
}

PlayerProfile::~PlayerProfile() {
}

PlayerProfile& PlayerProfile::operator=(const PlayerProfile& other) {
   PlayerProfile tmp(other);
   this->swap(tmp);
   return *this;
}

void PlayerProfile::swap(PlayerProfile& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->player_id_, other.player_id_);
}

Player::PlayerId PlayerProfile::player_id() const {
   return this->player_id_;
}

void PlayerProfile::player_id(Player::PlayerId player_id) {
   this->player_id_ = player_id;
}
