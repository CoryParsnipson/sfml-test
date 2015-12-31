#include "IsoMapBuilder.h"

#include "Serializer.h"

#include "TileFactory.h"
#include "TextureManager.h"

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
      Serializer::data_t d = this->serializer_->get();

      // need entity type identifier?
      this->build_tile(std::stoi(d["pos_x"]), std::stoi(d["pos_y"]), d["texture"]);
   }
}

void IsoMapBuilder::build_map() {
   this->map_ = new IsoMap();
}

void IsoMapBuilder::build_tile(int x, int y, std::string texture) {
   if (!this->map_) {
      this->build_map();
   }

   this->map_->set_tile(x, y, TileFactory::inst()->create_tile(this->texture_manager_.get_texture(texture)));
}
