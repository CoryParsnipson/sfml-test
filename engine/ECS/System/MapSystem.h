#ifndef MAP_SYSTEM_H
#define MAP_SYSTEM_H

#include <memory>
#include <string>

#include "GraphicalSystem.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;
class RenderSurface;

// ----------------------------------------------------------------------------
// MapSystem
//
// This system works on TileMap components. TileMap components store an array
// of graphics that can be used to create backgrounds and maps.
// ----------------------------------------------------------------------------
class MapSystem : public GraphicalSystem {
public:
   MapSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera);
   virtual ~MapSystem();

private:
   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
