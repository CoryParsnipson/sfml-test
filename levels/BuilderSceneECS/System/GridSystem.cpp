#include "GridSystem.h"

#include "Scene.h"

#include "Space.h"
#include "Rectangle.h"
#include "Grid2.h"

GridSystem::GridSystem(const std::string& id, CameraPtr camera)
: camera_(camera)
{
}

GridSystem::~GridSystem() {
}

CameraPtr GridSystem::camera() {
   return this->camera_;
}

void GridSystem::on_init(Game& game) {
   this->subscribe_to().all_of<Space, Grid2>();
}

void GridSystem::on_update(Game& game, Entity& e) {
   std::map<unsigned int, Entity*> cols;
   std::map<unsigned int, Entity*> rows;

   std::string col_str = e.get<Grid2>()->id() + "_col";
   std::string row_str = e.get<Grid2>()->id() + "_row";

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
   }
   
   // find out how many gridlines there should be based on the camera and update them
   sf::FloatRect camera_bounds = this->camera()->bounds();
   unsigned int gridline_id = 0;
   for (int c = camera_bounds.left; c <= camera_bounds.left + camera_bounds.width; c += e.get<Grid2>()->tile_width()) {
      if (cols[gridline_id] == nullptr || cols[gridline_id]->get<Rectangle>() == nullptr) {
         if (cols[gridline_id] != nullptr && cols[gridline_id]->get<Rectangle>() == nullptr) {
            this->scene().remove_entity(cols[gridline_id]->handle());
         }

         // create new column
         this->create_col(&e, col_str + std::to_string(gridline_id), c);
      } else {
         // modify column
         Rectangle* gridline = cols[gridline_id]->get<Rectangle>();
         gridline->position(c, camera_bounds.top);
         gridline->size(1, camera_bounds.height);
      }

      ++gridline_id;
   }

   gridline_id = 0;
   for (int r = camera_bounds.top; r <= camera_bounds.top + camera_bounds.height; r += e.get<Grid2>()->tile_height()) {
      if (rows[gridline_id] == nullptr || rows[gridline_id]->get<Rectangle>() == nullptr) {
         if (rows[gridline_id] != nullptr && rows[gridline_id]->get<Rectangle>() == nullptr) {
            this->scene().remove_entity(cols[gridline_id]->handle());
         }

         // create new row
         this->create_row(&e, row_str + std::to_string(gridline_id), r);
      } else {
         // modify row
         Rectangle* gridline = rows[gridline_id]->get<Rectangle>();
         gridline->position(camera_bounds.left, r);
         gridline->size(camera_bounds.width, 1);
      }

      ++gridline_id;
   }
}

void GridSystem::create_gridline(Entity* grid_entity, std::string id, float x, float y, float width, float height) {
   Entity* e = this->scene().get_entity(this->scene().create_entity());
   this->send_message_async<AddToEntityMessage>(grid_entity->handle(), e->handle());

   e->id(id);
   e->add<Rectangle>(id, x, y, width, height);
   e->get<Rectangle>()->color(sf::Color(230, 230, 230, 90));
}

void GridSystem::create_col(Entity* grid_entity, std::string id, float x) {
   this->create_gridline(grid_entity, id, x, this->camera()->bounds().top, 1, this->camera()->bounds().height);
}

void GridSystem::create_row(Entity* grid_entity, std::string id, float y) {
   this->create_gridline(grid_entity, id, this->camera()->bounds().left, y, this->camera()->bounds().width, 1);
}
