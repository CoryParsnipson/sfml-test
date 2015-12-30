#include "FlatMapBuilder.h"

#include "TextSerializer.h"

#include "TileFactory.h"
#include "TextureManager.h"

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
      this->build_tile(std::stoi(d["pos_x"]), std::stoi(d["pos_y"]), std::stoi(d["texture"]));
   }
}

void FlatMapBuilder::build_map() {
   this->map_ = new FlatMap();
}

void FlatMapBuilder::build_tile(int x, int y, int texture) {
   if (!this->map_) {
      this->build_map();
   }

   this->map_->set_tile(x, y, TileFactory::inst()->create_tile(*this->texture_manager_.get_texture(texture)));
}
