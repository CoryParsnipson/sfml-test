#include <cmath>

#include "GridSystem.h"
#include "Scene.h"
#include "Color.h"

#include "Space.h"
#include "Rectangle.h"
#include "Text.h"
#include "Grid.h"

#include "SetGridVisibilityMessage.h"

GridSystem::GridSystem(const std::string& id, CameraPtr camera)
: System(id)
, is_visible_(true)
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

void GridSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Space, Grid>();

   // register grid visibility toggling
   this->install_message_handler<SetGridVisibilityMessage>([&] (SetGridVisibilityMessage& msg) {
      Game::logger().msg(this->id(), Logger::INFO, "Handling SetGridVisibilityMessage.");

      Entity* grid_entity = this->scene().get_entity(msg.grid_entity);
      if (grid_entity) {
         Space* grid_entity_space = grid_entity->get<Space>();
         for (unsigned int i = 0; i < grid_entity_space->num_children(); ++i) {
            Entity* e = this->scene().get_entity(grid_entity_space->get(i));
            e->get<Space>()->visible(msg.visibility);
         }
      }

      this->is_visible_ = msg.visibility;
   });
}

void GridSystem::on_update(Game& game, Entity& e) {
   std::map<unsigned int, Entity*> cols;
   std::map<unsigned int, Entity*> rows;
   std::map<unsigned int, Entity*> text_markers;

   std::string col_str = e.get<Grid>()->id() + "_col";
   std::string row_str = e.get<Grid>()->id() + "_row";
   std::string tm_str = e.get<Grid>()->id() + "_tm";

   sf::FloatRect camera_bounds = this->global_transform(e).getInverse().transformRect(this->camera()->bounds());

   // no need to update this system if the camera hasn't changed
   if (this->previous_camera_bounds_ == camera_bounds && this->previous_zoom_factor_ == e.get<Grid>()->zoom_factor) {
      return;
   }
   this->previous_camera_bounds_ = camera_bounds;
   this->previous_zoom_factor_ = e.get<Grid>()->zoom_factor;

   // iterate through children and build list of existing rows and cols
   Space* space = e.get<Space>();
   for (unsigned int i = 0; i < space->num_children(); ++i) {
      Entity* gridline_entity = this->scene().get_entity(space->get(i));
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
   float tile_width = e.get<Grid>()->tile_width() * e.get<Grid>()->zoom_factor.x;
   float tile_height = e.get<Grid>()->tile_height() * e.get<Grid>()->zoom_factor.y;

   sf::Vector2f start_point = e.get<Grid>()->ceil(sf::Vector2f(camera_bounds.left - tile_width, camera_bounds.top - tile_height));
   sf::Vector2f end_point = e.get<Grid>()->ceil(sf::Vector2f(camera_bounds.left + camera_bounds.width + tile_width, camera_bounds.top + camera_bounds.height + tile_height));

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
         gridline->position(c, camera_bounds.top);
         gridline->size(1, camera_bounds.height);

         cols[gridline_id]->get<Space>()->visible(this->is_visible_);
      }

      ++gridline_id;
   }

   // set the extraneous columns to be invisible
   for (unsigned int c = gridline_id; c < cols.size(); ++c) {
      cols[c]->get<Space>()->visible(false);
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
         gridline->position(camera_bounds.left, r);
         gridline->size(camera_bounds.width, 1);

         rows[gridline_id]->get<Space>()->visible(this->is_visible_);
      }

      ++gridline_id;
   }

   // set the extraneous rows to be invisible
   for (unsigned int r = gridline_id; r < rows.size(); ++r) {
      rows[r]->get<Space>()->visible(false);
   }

   // make the text marker interval inversely dependent on grid zoom
   unsigned int interval = std::max(1, static_cast<int>(6 / e.get<Grid>()->zoom_factor.x));

   // update the text markers
   gridline_id = 0;
   std::map<unsigned int, Entity*>::iterator ty;
   std::map<unsigned int, Entity*>::iterator tx;
   for (ty = rows.begin(); ty != rows.end(); ++ty) {
      for (tx = cols.begin(); tx != cols.end(); ++tx) {
         float col_pos = tx->second->get<Rectangle>()->position().x;
         float row_pos = ty->second->get<Rectangle>()->position().y;

         sf::Vector2i grid_idx = e.get<Grid>()->grid_index(col_pos, row_pos);

         if (!ty->second->get<Space>()->visible() || !tx->second->get<Space>()->visible() || std::abs(grid_idx.y) % interval != 0 || std::abs(grid_idx.x) % interval != 0) {
            continue;
         }

         sf::Vector2f tm_val(col_pos, row_pos);
         sf::Vector2f tm_pos = tm_val + sf::Vector2f(3, 3); // offset the text marker a little to the lower right

         if (text_markers[gridline_id] == nullptr || text_markers[gridline_id]->get<Text>() == nullptr) {
            if (text_markers[gridline_id] != nullptr && text_markers[gridline_id]->get<Text>() == nullptr) {
               this->scene().remove_entity(text_markers[gridline_id]->handle());
            }

            // create new text marker
            text_markers[gridline_id] = this->create_text_marker(&e, tm_str + std::to_string(gridline_id), tm_pos);
         }

         // modify text marker
         text_markers[gridline_id]->get<Text>()->string(
            std::to_string((int)(grid_idx.x * e.get<Grid>()->tile_width())) +
            ", " +
            std::to_string((int)(grid_idx.y * e.get<Grid>()->tile_height()))
         );
         text_markers[gridline_id]->get<Text>()->position(tm_pos);

         text_markers[gridline_id]->get<Space>()->visible(this->is_visible_);

         ++gridline_id;
      }
   }

   // set the extraneous text markers to be invisible
   for (unsigned int t = gridline_id; t < text_markers.size(); ++t) {
      text_markers[t]->get<Space>()->visible(false);
   }

}

Entity* GridSystem::create_gridline(Entity* grid_entity, std::string id, float x, float y, float width, float height) {
   Entity* e = this->scene().create_entity();
   this->send_message<AddToEntityMessage>(grid_entity->handle(), e->handle());

   e->id(id);
   e->add<Rectangle>(id, x, y, width, height);
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
   Entity* e = this->scene().create_entity();
   this->send_message<AddToEntityMessage>(grid_entity->handle(), e->handle());
   
   e->id(id);
   e->add<Text>(id + "Text_Component", "", this->scene().fonts().get("retro"), 9);

   return e;
}
