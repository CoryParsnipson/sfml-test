#ifndef GRAPHIC_SYSTEM_H
#define GRAPHIC_SYSTEM_H

#include <vector>

#include "System.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;
class RenderSurface;

// ----------------------------------------------------------------------------
// GraphicSystem
//
// This system is enabled by default and renders all objects with graphics
// components to the specified window.
// ----------------------------------------------------------------------------
class GraphicSystem : public System {
public:
   GraphicSystem(const std::string& id, RenderSurface& surface);
   virtual ~GraphicSystem();

   Camera* camera(unsigned int idx = 0) const;

private:
   RenderSurface* surface_;
   std::vector<Camera*> cameras_;

   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
