#ifndef PLAYER_INPUT_SYSTEM_H
#define PLAYER_INPUT_SYSTEM_H

#include <string>

#include "System.h"

// ----------------------------------------------------------------------------
// PlayerInputSystem
//
// This is a test system that looks for anything with input compoennts
// and performs actions based on which intents are set.
// ----------------------------------------------------------------------------
class PlayerInputSystem : public System {
public:
   PlayerInputSystem(const std::string& id = "PlayerInputSystem");
   virtual ~PlayerInputSystem();

private:
   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
