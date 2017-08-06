#ifndef SPATIAL_SYSTEM_H
#define SPATIAL_SYSTEM_H

#include "System.h"

// ----------------------------------------------------------------------------
// SpatialSystem
//
// This system handles all operations pertaining to Space components and
// "Scene Graph" like functionality. Other systems should send AddToEntity
// or RemoveFromEntity messages to this system to modify the scene graph.
// ----------------------------------------------------------------------------
class SpatialSystem : public System {
public:
   SpatialSystem(const std::string& id = "SpatialSystem");
   virtual ~SpatialSystem();

private:
   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
