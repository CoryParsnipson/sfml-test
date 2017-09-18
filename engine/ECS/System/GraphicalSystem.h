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
// GraphicalSystem
//
// Systems that draw graphics to the screen should inherit from this system.
// GraphicalSystem provides a camera and a camera resize message handler.
// ----------------------------------------------------------------------------
class GraphicalSystem : public System {
public:
   GraphicalSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera);
   virtual ~GraphicalSystem();

   std::shared_ptr<Camera> camera() const;
   RenderSurface* surface() const;

private:
   RenderSurface* surface_;
   std::shared_ptr<Camera> camera_;

   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
