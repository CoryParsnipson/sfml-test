#include "IsoMapBuilder.h"

#include "TextSerializer.h"

#include "TileFactory.h"
#include "TextureManager.h"

IsoMapBuilder::IsoMapBuilder(TextureManager& tm)
: MapBuilder(tm)
{
}

IsoMapBuilder::~IsoMapBuilder() {
}

void IsoMapBuilder::create_serializer(std::string filename) {
   delete this->serializer_;

   this->serializer_ = new TextSerializer();
   this->serializer_->open(filename);
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
      this->build_tile(std::stoi(d["pos_x"]), std::stoi(d["pos_y"]), std::stoi(d["texture"]));
   }
}

void IsoMapBuilder::build_map() {
   this->map_ = new IsoMap();
}

void IsoMapBuilder::build_tile(int x, int y, int texture) {
   if (!this->map_) {
      this->build_map();
   }

   this->map_->set_tile(x, y, TileFactory::inst()->create_tile(*this->texture_manager_.get_texture(texture)));
}
