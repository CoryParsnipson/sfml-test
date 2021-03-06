#include <cmath>

#include "GridSystem.h"
#include "Scene.h"
#include "Color.h"

#include "Rectangle.h"
#include "Text.h"
#include "Grid.h"

#include "SetGridVisibilityMessage.h"

GridSystem::GridSystem(const std::string& id, CameraPtr camera)
: System(id)
, is_visible_(true)
, force_update_(false)
, camera_(camera)
, previous_camera_bounds_(0, 0, 0, 0)
, previous_zoom_factor_(1.f, 1.f)
{
}

GridSystem::~GridSystem() {
}

CameraPtr GridSystem::camera() {
   return this->camera_;
}

void GridSystem::force_update() {
   this->force_update_ = true;
}

void GridSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Grid>();

   // register grid visibility toggling
   this->install_message_handler<SetGridVisibilityMessage>([&] (SetGridVisibilityMessage& msg) {
      Game::logger().msg(this->id(), Logger::INFO, "Handling SetGridVisibilityMessage.");

      Entity* grid_entity = this->scene().get_entity(msg.grid_entity);
      if (grid_entity) {
         grid_entity->space()->visible(msg.visibility);
      }
      this->is_visible_ = msg.visibility;

      this->force_update_ = true;
      this->on_update(game, *grid_entity);
   });
}

void GridSystem::on_update(Game& game, Entity& e) {
   std::map<unsigned int, Entity*> cols;
   std::map<unsigned int, Entity*> rows;
   std::map<unsigned int, Entity*> text_markers;

   Grid* grid = e.get<Grid>();

   std::string col_str = grid->id() + "_col";
   std::string row_str = grid->id() + "_row";
   std::string tm_str = grid->id() + "_tm";

   sf::FloatRect camera_bounds = this->global_transform(e).getInverse().transformRect(this->camera()->bounds());

   // no need to update this system if the camera hasn't changed
   if (!this->force_update_ && this->previous_camera_bounds_ == camera_bounds && this->previous_zoom_factor_ == grid->zoom_factor()) {
      return;
   }
   this->previous_camera_bounds_ = camera_bounds;
   this->previous_zoom_factor_ = grid->zoom_factor();

   this->force_update_ = false;

   // iterate through children and build list of existing rows and cols
   SceneNode* space = e.space();
   for (unsigned int i = 0; i < space->num_children(); ++i) {
      Entity* gridline_entity = this->scene().get_entity(space->get_child(i)->entity());
      if (!gridline_entity) {
         continue;
      }

      if (gridline_entity->id().substr(0, col_str.size()) == col_str) {
         cols[std::stoi(gridline_entity->id().substr(col_str.size()))] = gridline_entity;
      }

      if (gridline_entity->id().substr(0, row_str.size()) == row_str) {
         rows[std::stoi(gridline_entity->id().substr(row_str.size()))] = gridline_entity;
      }

      if (gridline_entity->id().substr(0, tm_str.size()) == tm_str) {
         text_markers[std::stoi(gridline_entity->id().substr(tm_str.size()))] = gridline_entity;
      }
   }
   
   // find out how many gridlines there should be based on the camera and update them
   unsigned int gridline_id = 0;
   float tile_width = grid->tile_width() * grid->zoom_factor().x;
   float tile_height = grid->tile_height() * grid->zoom_factor().y;

   sf::Vector2f start_point = grid->ceil(sf::Vector2f(camera_bounds.left - tile_width, camera_bounds.top - tile_height));
   sf::Vector2f end_point = grid->ceil(sf::Vector2f(camera_bounds.left + camera_bounds.width + tile_width, camera_bounds.top + camera_bounds.height + tile_height));

   for (float c = start_point.x; c <= end_point.x; c += tile_width) {
      if (cols[gridline_id] == nullptr || cols[gridline_id]->get<Rectangle>() == nullptr) {
         if (cols[gridline_id] != nullptr && cols[gridline_id]->get<Rectangle>() == nullptr) {
            this->scene().remove_entity(cols[gridline_id]->handle());
         }

         // create new column
         cols[gridline_id] = this->create_col(&e, col_str + std::to_string(gridline_id), c);
      } else {
         // modify column
         Rectangle* gridline = cols[gridline_id]->get<Rectangle>();
         gridline->size(1, camera_bounds.height);

         cols[gridline_id]->space()->position(c, camera_bounds.top);
         cols[gridline_id]->space()->visible(this->is_visible_);
      }

      ++gridline_id;
   }

   // set the extraneous columns to be invisible
   for (unsigned int c = gridline_id; c < cols.size(); ++c) {
      cols[c]->space()->visible(false);
   }

   gridline_id = 0;
   for (float r = start_point.y; r <= end_point.y; r += tile_height) {
      if (rows[gridline_id] == nullptr || rows[gridline_id]->get<Rectangle>() == nullptr) {
         if (rows[gridline_id] != nullptr && rows[gridline_id]->get<Rectangle>() == nullptr) {
            this->scene().remove_entity(cols[gridline_id]->handle());
         }

         // create new row
         rows[gridline_id] = this->create_row(&e, row_str + std::to_string(gridline_id), r);
      } else {
         // modify row
         Rectangle* gridline = rows[gridline_id]->get<Rectangle>();
         gridline->size(camera_bounds.width, 1);

         rows[gridline_id]->space()->position(camera_bounds.left, r);
         rows[gridline_id]->space()->visible(this->is_visible_);
      }

      ++gridline_id;
   }

   // set the extraneous rows to be invisible
   for (unsigned int r = gridline_id; r < rows.size(); ++r) {
      rows[r]->space()->visible(false);
   }

   // make the text marker interval about every 200 pixels
   unsigned int interval = std::round(200 / (std::min(grid->tile_height(), grid->tile_width()) * grid->zoom_factor().x));

   // update the text markers
   gridline_id = 0;
   std::map<unsigned int, Entity*>::iterator ty;
   std::map<unsigned int, Entity*>::iterator tx;
   for (ty = rows.begin(); ty != rows.end(); ++ty) {
      for (tx = cols.begin(); tx != cols.end(); ++tx) {
         float col_pos = tx->second->space()->position().x;
         float row_pos = ty->second->space()->position().y;

         sf::Vector2f tm_pos(col_pos, row_pos);
         sf::Vector2i grid_idx = grid->grid_index(tm_pos);

         if (!ty->second->space()->visible() || !tx->second->space()->visible() || std::abs(grid_idx.y) % interval != 0 || std::abs(grid_idx.x) % interval != 0) {
            continue;
         }

         if (text_markers[gridline_id] == nullptr || text_markers[gridline_id]->get<Text>() == nullptr) {
            if (text_markers[gridline_id] != nullptr && text_markers[gridline_id]->get<Text>() == nullptr) {
               this->scene().remove_entity(text_markers[gridline_id]->handle());
            }

            // create new text marker
            text_markers[gridline_id] = this->create_text_marker(&e, tm_str + std::to_string(gridline_id), tm_pos);
         }

         // modify text marker
         text_markers[gridline_id]->get<Text>()->string(
            std::to_string((int)(grid_idx.x * grid->tile_width())) +
            ", " +
            std::to_string((int)(grid_idx.y * grid->tile_height()))
         );

         text_markers[gridline_id]->space()->position(tm_pos);
         text_markers[gridline_id]->space()->visible(this->is_visible_);

         ++gridline_id;
      }
   }

   // set the extraneous text markers to be invisible
   for (unsigned int t = gridline_id; t < text_markers.size(); ++t) {
      text_markers[t]->space()->visible(false);
   }

}

Entity* GridSystem::create_gridline(Entity* grid_entity, std::string id, float x, float y, float width, float height) {
   Entity* e = this->scene().create_entity(id);
   this->add_to_scene_node(grid_entity, e);

   e->space()->position(x, y);

   e->add<Rectangle>(id, 0, 0, width, height);
   e->get<Rectangle>()->color(Color(230, 230, 230, 90));

   return e;
}

Entity* GridSystem::create_col(Entity* grid_entity, std::string id, float x) {
   return this->create_gridline(grid_entity, id, x, this->camera()->bounds().top, 1, this->camera()->bounds().height);
}

Entity* GridSystem::create_row(Entity* grid_entity, std::string id, float y) {
   return this->create_gridline(grid_entity, id, this->camera()->bounds().left, y, this->camera()->bounds().width, 1);
}

Entity* GridSystem::create_text_marker(Entity* grid_entity, std::string id, const sf::Vector2f& pos) {
   Entity* e = this->scene().create_entity(id);
   this->add_to_scene_node(grid_entity, e);
   
   e->add<Text>(id + "Text_Component", "", this->scene().fonts().get("retro"), 9);
   e->get<Text>()->offset(3, 3);

   return e;
}
