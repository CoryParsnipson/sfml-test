#include "DefaultMouseGamepadPreset.h"

#include "LeftMouseClickCommand.h"
#include "LeftMouseReleaseCommand.h"
#include "RightMouseClickCommand.h"
#include "RightMouseReleaseCommand.h"
#include "MouseMoveCommand.h"
#include "MouseWheelCommand.h"

DefaultMouseGamepadPreset::DefaultMouseGamepadPreset(SceneObject& scene_graph)
: scene_graph_(&scene_graph)
{
}

DefaultMouseGamepadPreset::~DefaultMouseGamepadPreset() {
}

void DefaultMouseGamepadPreset::apply(PlayerGamepad* pg) {
   if (!pg) {
      return;
   }

   pg->set(new LeftMouseClickCommand(pg, this->scene_graph_), MouseButton::Left, ButtonState::Pressed);
   pg->set(new RightMouseClickCommand(pg, this->scene_graph_), MouseButton::Right, ButtonState::Pressed);

   pg->set(new LeftMouseReleaseCommand(pg, this->scene_graph_), MouseButton::Left, ButtonState::Released);
   pg->set(new RightMouseReleaseCommand(pg, this->scene_graph_), MouseButton::Right, ButtonState::Released);

   pg->set(new MouseMoveCommand(pg, this->scene_graph_), MouseAction::Move);
   pg->set(new MouseWheelCommand(pg, this->scene_graph_), MouseAction::Wheel);
}
