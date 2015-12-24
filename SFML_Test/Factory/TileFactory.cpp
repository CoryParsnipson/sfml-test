#include "TileFactory.h"

#include "GraphicsPart.h"
#include "PhysicsPart.h"

// initialize static members
TileFactory* TileFactory::inst_ = new TileFactory();

TileFactory* TileFactory::inst() {
   return TileFactory::inst_;
}

TileFactory::TileFactory() {
}

TileFactory::~TileFactory() {
}

Entity2* TileFactory::create_tile(Texture& t) {
   return this->create_tile(t, sf::Vector2f(0, 0));
}

Entity2* TileFactory::create_tile(Texture& t, sf::Vector2f pos) {
   Entity2* tile = new Entity2();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart();

   graphics_part->add(new sf::Sprite(t.get_texture()));
   
   physics_part->set_position(pos);
  
   tile->add(graphics_part);
   tile->add(physics_part); 

   return tile;
}
