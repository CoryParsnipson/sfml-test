#include "SwitchSceneCommand.h"
#include "Game.h"
#include "Scene.h"

SwitchSceneCommand::SwitchSceneCommand(Scene* scene /* = nullptr */)
: next_scene_(scene)
{
}

SwitchSceneCommand::~SwitchSceneCommand() {
}

void SwitchSceneCommand::execute() {
   if (!this->next_scene_) {
      return;
   }

   Game::instance()->switch_scene(this->next_scene_);
}

void SwitchSceneCommand::unexecute() {
   // implement me
}
