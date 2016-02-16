#include "TileFactory.h"

#include "UtilFactory.h"

#include "Graphic.h"

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

   graphics_part->add(new Sprite(t));
   physics_part->set_size(t.get_size());
  
   tile->add(graphics_part);
   tile->add(physics_part); 

   tile->set_position(pos);

   return tile;
}

Entity* TileFactory::create_tile_cursor(sf::Vector2f& one, sf::Vector2f& two, std::vector<Entity*> references) {
   Entity* cursor = new Entity();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart();
   ReferencePart* ref_part = new ReferencePart();

   sf::FloatRect* rect = UtilFactory::inst()->create_float_rect(one, two);
   
   Shape* cursor_graphic = new Shape(new sf::RectangleShape());
   cursor_graphic->set_size(rect->width, rect->height);
   cursor_graphic->set_fill_color(sf::Color(225, 225, 225, 128));
   cursor_graphic->set_outline_color(sf::Color(255, 255, 255, 192));
   cursor_graphic->set_outline_thickness(2.0);

   graphics_part->add(cursor_graphic);
   physics_part->set_size(rect->width, rect->height);

   std::vector<Entity*>::const_iterator it;
   for (it = references.begin(); it != references.end(); ++it) {
      ref_part->add(*it);
   }

   cursor->add(graphics_part);
   cursor->add(physics_part);
   cursor->add(ref_part);

   cursor->set_position(rect->left, rect->top);

   delete rect;
   rect = nullptr;

   return cursor;
}

Entity* TileFactory::create_selection_rectangle(sf::FloatRect* bounds) {
   Entity* sr = new Entity();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart();
   
   Graphic* sr_graphic = new Shape(new sf::RectangleShape());
   sr_graphic->set_fill_color(sf::Color(66, 108, 167, 175));
   sr_graphic->set_outline_color(sf::Color(124, 160, 210, 192));
   sr_graphic->set_outline_thickness(1.0);

   graphics_part->add(sr_graphic);

   sr->add(graphics_part);
   sr->add(physics_part);

   sr->set_position(bounds ? bounds->left : 0, bounds ? bounds->top : 0);
   sr->set_size(bounds ? bounds->width : 0, bounds ? bounds->height : 0);

   return sr;
}
