#ifndef GAMEPAD_PRESET_H
#define GAMEPAD_PRESET_H

#include "PlayerGamepad.h"

class GamepadPreset {
public:
   virtual void apply(PlayerGamepad* pg) = 0;
};

#endif
