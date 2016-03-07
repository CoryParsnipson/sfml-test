#include "FlatMapBuilder.h"

#include "TextSerializer.h"

#include "TileFactory.h"
#include "TextureManager.h"

#include "OrthographicGrid.h"

#include "PhysicsPart.h"

FlatMapBuilder::FlatMapBuilder(TextureManager& tm)
: MapBuilder(tm)
{
}

FlatMapBuilder::~FlatMapBuilder() {
}

void FlatMapBuilder::build() {
   this->build_map();

   if (!this->serializer_) {
      Service::get_logger().msg("FlatMapBuilder", Logger::ERROR, "Serializer has not been initialized!");
      return;
   }

   while (this->serializer_->next()) {
      Serializer::SerializedObj d = this->serializer_->get();

      if (d["type"] == "grid") {
         Grid* grid = nullptr;
         this->serializer_->deserialize(d, grid);
         this->build_grid(grid);
      } else if (d["type"] == "layer") {
         // TODO: do this later
      } else {
         // assume it's an Entity
         Entity* entity = nullptr;
         this->serializer_->deserialize(d, entity);

         if (d["type"] == "tile") {
            this->build_tile(entity);
         }
      }
   }
}

void FlatMapBuilder::build_map() {
   this->map_ = new Map();
}

void FlatMapBuilder::build_tile(Entity* tile) {
   if (!this->map_) {
      this->build_map();
   }

   if (!tile) {
      Service::get_logger().msg("FlatMapBuilder", Logger::WARNING, "Received null tile pointer.");
      return;
   }

   this->map_->add(tile);
}

void FlatMapBuilder::build_grid(Grid* grid) {
   if (!this->map_) {
      this->build_map();
   }

   if (!grid) {
      Service::get_logger().msg("FlatMapBuilder", Logger::WARNING, "Received null grid pointer.");
      return;
   }

   this->map_->add(grid);
}
