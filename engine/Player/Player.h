#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "InputBinding.h"

// ----------------------------------------------------------------------------
// Player
//
// This class is used to manage player profile information such as input
// bindings, savedata, etc.
// ----------------------------------------------------------------------------
class Player {
public:
   using PlayerId = unsigned int;

   Player(PlayerId id, const std::string& name = "Player");
   ~Player();
   
   PlayerId id() const;
   void id(PlayerId id);

   std::string name() const;
   void name(const std::string& name);

   InputBinding& bindings();

private:
   PlayerId id_;
   std::string name_;

   InputBinding binding_;
};

#endif
