#ifndef MAP_SYSTEM_H
#define MAP_SYSTEM_H

#include <memory>
#include <string>

#include "System.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;
class RenderSurface;

// ----------------------------------------------------------------------------
// MapSystem
//
// This system is responsible for updating the map
// ----------------------------------------------------------------------------
class MapSystem : public System {
public:
   MapSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera);
   virtual ~MapSystem();

   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);

private:
   RenderSurface* surface_;
   std::shared_ptr<Camera> camera_;
};

#endif
