#include "IsoMapBuilder.h"

#include "Serializer.h"

#include "TileFactory.h"
#include "TextureManager.h"

#include "PhysicsPart.h"

IsoMapBuilder::IsoMapBuilder(TextureManager& tm)
: MapBuilder(tm)
{
}

IsoMapBuilder::~IsoMapBuilder() {
}

void IsoMapBuilder::build() {
   this->build_map();

   if (!this->serializer_) {
      Service::get_logger().msg("IsoMapBuilder", Logger::ERROR, "Serializer has not been initialized!");
      return;
   }

   while (this->serializer_->next()) {
      Serializer::SerializedObj d = this->serializer_->get();

      // need entity type identifier?
      this->build_tile(std::stoi(d["pos_x"]), std::stoi(d["pos_y"]), d["texture"]);
   }
}

void IsoMapBuilder::build_map() {
   this->map_ = new Map();
}

void IsoMapBuilder::build_tile(int x, int y, std::string texture) {
   if (!this->map_) {
      this->build_map();
   }

   Entity* tile = TileFactory::inst()->create_tile(this->texture_manager_.get_texture(texture));

   PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>(tile->get("physics"));
   if (tile_physics) {
      int pos_x = x * Settings::Instance()->TILE_WIDTH + (y & 1) * (Settings::Instance()->TILE_WIDTH / 2);
      int pos_y = y * (Settings::Instance()->TILE_HEIGHT_RHOMBUS / 2) - Settings::Instance()->TILE_HEIGHT_OVERLAP;

      tile_physics->set_position(pos_x, pos_y);
   } else {
      Service::get_logger().msg("IsoMap", Logger::WARNING, "Tile has no physics component.");
   }

   this->map_->add(tile);
}

void IsoMapBuilder::build_grid(const std::string& id, int tile_size) {
   if (!this->map_) {
      this->build_map();
   }

   // TODO: this should be IsomorphicGrid
   //this->map_->add(new Grid(id, tile_size));
}

void IsoMapBuilder::build_grid(const std::string& id, const sf::Vector2f& tile_size) {
   if (!this->map_) {
      this->build_map();
   }

   // TODO: this should be IsomorphicGrid
   //this->map_->add(new Grid(id, tile_size));
}

void IsoMapBuilder::build_grid(const std::string& id, const sf::Vector2f& tile_size, const sf::Vector2f& origin) {
   if (!this->map_) {
      this->build_map();
   }

   // TODO: this should be IsomorphicGrid
   //this->map_->add(new Grid(id, tile_size));
   //this->map_->grid()->origin(origin);
}
