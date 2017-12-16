#ifndef PLAYER_PROFILE_H
#define PLAYER_PROFILE_H

#include "Component.h"
#include "Player.h"

// ----------------------------------------------------------------------------
// PlayerProfile
//
// Attach this component to an entity to associate it with a specific player
// profile.
// ----------------------------------------------------------------------------
class PlayerProfile : public Component {
public:
   explicit PlayerProfile(const std::string& id = "PlayerProfile Component");
   PlayerProfile(const PlayerProfile& other);
   virtual ~PlayerProfile();
   
   PlayerProfile& operator=(const PlayerProfile& other);
   void swap(PlayerProfile& other);

   Player::PlayerId player_id() const;
   void player_id(Player::PlayerId player_id);

private:
   Player::PlayerId player_id_;
};

#endif
