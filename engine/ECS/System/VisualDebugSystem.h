#ifndef VISUAL_DEBUG_SYSTEM_H
#define VISUAL_DEBUG_SYSTEM_H

#include "GraphicalSystem.h"

// ----------------------------------------------------------------------------
// VisualDebugSystem
//
// This system highlights certain components that usually don't have graphical
// representations. This system also has options to draw input element
// representations to show the state of buttons/axes.
// ----------------------------------------------------------------------------
class VisualDebugSystem : public GraphicalSystem {
public:
   VisualDebugSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera);
   virtual ~VisualDebugSystem();

private:
   virtual void on_init(Game& game);

   virtual void pre_update(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
