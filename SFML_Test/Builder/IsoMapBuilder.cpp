#include "IsoMapBuilder.h"

#include "TileFactory.h"
#include "TextureManager.h"

IsoMapBuilder::IsoMapBuilder(TextureManager& tm)
: MapBuilder(tm)
{
}

IsoMapBuilder::~IsoMapBuilder() {
}

void IsoMapBuilder::create_map() {
   this->map_ = new IsoMap();
}

Entity* IsoMapBuilder::load_tile() {
   if (!this->map_) {
      this->create_map();
   }

   // add temporary test tiles
   this->map_->set_tile(3, 3, TileFactory::inst()->create_tile(*this->texture_manager_.get_texture(1)));
   this->map_->set_tile(3, 4, TileFactory::inst()->create_tile(*this->texture_manager_.get_texture(0)));
   this->map_->set_tile(3, 5, TileFactory::inst()->create_tile(*this->texture_manager_.get_texture(1)));
   this->map_->set_tile(4, 4, TileFactory::inst()->create_tile(*this->texture_manager_.get_texture(0)));

   return nullptr;
}
