#ifndef GRAPHIC_SYSTEM_H
#define GRAPHIC_SYSTEM_H

#include <vector>
#include <memory>

#include "System.h"
#include "Camera.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class RenderSurface;

// ----------------------------------------------------------------------------
// GraphicalSystem
//
// Systems that draw graphics to the screen should inherit from this system.
// GraphicalSystem provides a camera and a camera resize message handler.
// ----------------------------------------------------------------------------
class GraphicalSystem : public System {
public:
   GraphicalSystem(const std::string& id, RenderSurface& surface, CameraPtr camera);
   virtual ~GraphicalSystem();

   CameraPtr camera() const;
   RenderSurface* surface() const;

private:
   RenderSurface* surface_;
   CameraPtr camera_;

   virtual void on_init(Game& game);

   virtual void pre_update(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
