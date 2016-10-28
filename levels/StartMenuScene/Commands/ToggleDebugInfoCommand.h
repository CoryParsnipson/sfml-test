#ifndef TOGGLE_DEBUG_INFO_COMMAND_H
#define TOGGLE_DEBUG_INFO_COMMAND_H

#include "sfml.h"
#include "Command.h"
#include "DebugVisitor.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Entity;
class SceneObject;

// ----------------------------------------------------------------------------
// ToggleDebugInfoCommand
//
// Visit the provided scene graph and add/remove debug parts to all the
// entities.
// ----------------------------------------------------------------------------
class ToggleDebugInfoCommand : public Command {
public:
   ToggleDebugInfoCommand(SceneObject* scene_graph, sf::Font* font);

   virtual void execute();
   virtual void unexecute();

private:
   sf::Font* font_;
   SceneObject* scene_graph_;

   DebugVisitor debug_visitor_;
};

#endif
