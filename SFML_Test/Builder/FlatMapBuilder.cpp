#include "FlatMapBuilder.h"

#include "TextSerializer.h"

#include "TileFactory.h"
#include "TextureManager.h"

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
      Serializer::data_t d = this->serializer_->get();

      // need entity type identifier?
      this->build_tile(std::stoi(d["pos_x"]), std::stoi(d["pos_y"]), d["texture"]);
   }
}

void FlatMapBuilder::build_map() {
   this->map_ = new Map();
}

void FlatMapBuilder::build_tile(int x, int y, std::string texture) {
   if (!this->map_) {
      this->build_map();
   }
   
   Entity* tile = TileFactory::inst()->create_tile(this->texture_manager_.get_texture(texture));

   PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>(tile->get("physics"));
   if (tile_physics) {
      tile_physics->set_position(x * Settings::Instance()->TILE_WIDTH, y * Settings::Instance()->TILE_HEIGHT);
   } else {
      Service::get_logger().msg("FlatMap", Logger::WARNING, "Tile has no physics component.");
   }

   this->map_->add(x, y, tile);
}
