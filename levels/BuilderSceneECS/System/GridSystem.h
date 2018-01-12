#ifndef GRID_SYSTEM_H
#define GRID_SYSTEM_H

#include <vector>

#include "System.h"
#include "Camera.h"

// ----------------------------------------------------------------------------
// GridSystem
//
// This system creates a grid overlay for map building and stuff.
// ----------------------------------------------------------------------------
class GridSystem : public System {
public:
   GridSystem(const std::string& id, CameraPtr camera);
   virtual ~GridSystem();

   CameraPtr camera();

private:
   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);

   void create_gridline(Entity* grid_entity, std::string id, float x, float y, float width, float height);

   void create_col(Entity* grid_entity, std::string id, float x);
   void create_row(Entity* grid_entity, std::string id, float y);

   CameraPtr camera_;
};

#endif
