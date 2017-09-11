#ifndef GRAPHIC_SYSTEM_H
#define GRAPHIC_SYSTEM_H

#include <vector>
#include <memory>

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
   GraphicSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera);
   virtual ~GraphicSystem();

   std::shared_ptr<Camera> camera() const;

private:
   RenderSurface* surface_;
   std::shared_ptr<Camera> camera_;

   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
