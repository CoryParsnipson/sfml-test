#include "TileFactory.h"

#include "GraphicsPart.h"
#include "PhysicsPart.h"
#include "ReferencePart.h"

// initialize static members
TileFactory* TileFactory::inst_ = new TileFactory();

TileFactory* TileFactory::inst() {
   return TileFactory::inst_;
}

TileFactory::TileFactory() {
}

TileFactory::~TileFactory() {
}

Entity* TileFactory::create_tile(Texture& t) {
   return this->create_tile(t, sf::Vector2f(0, 0));
}

Entity* TileFactory::create_tile(Texture& t, sf::Vector2f pos) {
   Entity* tile = new Entity();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart();

   graphics_part->add(new sf::Sprite(t.get_texture()));
   
   physics_part->set_position(pos);
   physics_part->set_size(t.get_size());
  
   tile->add(graphics_part);
   tile->add(physics_part); 

   return tile;
}

Entity* TileFactory::create_tile_cursor(sf::Vector2f pos, sf::Vector2f size, std::vector<Entity*> references) {
   Entity* cursor = new Entity();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart();
   ReferencePart* ref_part = new ReferencePart();

   // TODO: come back to this and do it better
   sf::RectangleShape* cursor_graphic = new sf::RectangleShape();
   cursor_graphic->setSize(size);
   cursor_graphic->setFillColor(sf::Color(225, 225, 225, 128));
   cursor_graphic->setOutlineColor(sf::Color(255, 255, 255, 192));
   cursor_graphic->setOutlineThickness(2.0);
   cursor_graphic->setPosition(pos);

   graphics_part->add(cursor_graphic);

   physics_part->set_position(pos);
   physics_part->set_size(size);

   std::vector<Entity*>::const_iterator it;
   for (it = references.begin(); it != references.end(); ++it) {
      ref_part->add(*it);
   }

   cursor->add(graphics_part);
   cursor->add(physics_part);
   cursor->add(ref_part);

   return cursor;
}
