#ifndef DEFAULT_MOUSE_GAMEPAD_PRESET_H
#define DEFAULT_MOUSE_GAMEPAD_PRESET_H

#include "GamepadPreset.h"

class DefaultMouseGamepadPreset : public GamepadPreset {
public:
   DefaultMouseGamepadPreset(SceneObject& scene_graph);
   ~DefaultMouseGamepadPreset();

   virtual void apply(PlayerGamepad* pg);

protected:
   SceneObject* scene_graph_;
};

#endif
