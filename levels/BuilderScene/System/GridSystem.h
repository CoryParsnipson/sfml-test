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

   Entity* create_gridline(Entity* grid_entity, std::string id, float x, float y, float width, float height);

   Entity* create_col(Entity* grid_entity, std::string id, float x);
   Entity* create_row(Entity* grid_entity, std::string id, float y);
   Entity* create_text_marker(Entity* grid_entity, std::string id, const sf::Vector2f& pos);

   bool is_visible_;

   CameraPtr camera_;
   sf::FloatRect previous_camera_bounds_;
};

#endif
