#include "OrthographicMapBuilder.h"

#include "TextSerializer.h"
#include "OrthographicGrid.h"
#include "TileFactory.h"
#include "PhysicsPart.h"

OrthographicMapBuilder::OrthographicMapBuilder()
: MapBuilder()
{
}

OrthographicMapBuilder::~OrthographicMapBuilder() {
}

void OrthographicMapBuilder::build() {
   this->build_map();

   if (!this->serializer_) {
      Service::get_logger().msg("OrthographicMapBuilder", Logger::ERROR, "Serializer has not been initialized!");
      return;
   }

   while (this->serializer_->next()) {
      Serialize::SerialObj d = this->serializer_->get();

      try {
         if (d["type"] == "grid") {
            Grid* grid;
            if (d["class"] == "OrthographicGrid") {
               grid = new OrthographicGrid("");
               grid->deserialize(d);
               grid->font(this->grid_font_);
               this->build_grid(grid);
            }
         } else if (d["type"] == "entity") {
            Entity* entity = new Entity();
            entity->deserialize(d, this->textures_);
            this->build_tile(entity);
         } else {
            Service::get_logger().msg("OrthographicMapBuilder", Logger::WARNING, "Received serialized object of unknown type '" + d["type"] + "'");
            continue;
         }
      } catch (const std::out_of_range& e) {
         Service::get_logger().msg("OrthographicMapBuilder", Logger::WARNING, "Unable to de-serialize object: " + d["type"]);
         continue;
      }
   }
}

void OrthographicMapBuilder::build_map() {
   this->map_ = new Map();
}

void OrthographicMapBuilder::build_tile(Entity* tile) {
   if (!this->map_) {
      this->build_map();
   }

   if (!tile) {
      Service::get_logger().msg("OrthographicMapBuilder", Logger::WARNING, "Received null tile pointer.");
      return;
   }

   this->map_->add(tile);
}

void OrthographicMapBuilder::build_grid(Grid* grid) {
   if (!this->map_) {
      this->build_map();
   }

   if (!grid) {
      Service::get_logger().msg("OrthographicMapBuilder", Logger::WARNING, "Received null grid pointer.");
      return;
   }

   this->map_->add(grid);
}
