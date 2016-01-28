#include "GraphicsPart.h"

#include "FontConfig.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"

GraphicsPart::GraphicsPart(std::string id)
: Part(id)
, show_outline_(false)
, show_debug_text_(false)
, font_debug_("retro", 11, FontConfig::ALIGN::LEFT)
{
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Creating GraphicsPart");
}

GraphicsPart::~GraphicsPart() {
   // TODO: probably need to delete everything in these arrays
   this->sprites_.clear();
   this->shapes_.clear();
}

void GraphicsPart::add(sf::Sprite* sprite) {
   this->sprites_.push_back(sprite);
}

void GraphicsPart::add(sf::Shape* shape) {
   this->shapes_.push_back(shape);
}

void GraphicsPart::set_show_outline(bool show) {
   this->show_outline_ = show;
}

void GraphicsPart::set_show_debug_text(bool show) {
   this->show_debug_text_ = show;
}

bool GraphicsPart::get_show_debug_text() {
   return this->show_debug_text_;
}

void GraphicsPart::update(Entity& entity, Scene& scene, Viewport& viewport) {
   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity.get("physics"));

   sprite_list_t::iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); sprite_it++) {
      // update graphics draw location based on physical position of entity
      if (physics) {
         (*sprite_it)->setPosition(physics->get_position());
      }

      viewport.draw(*(*sprite_it));

      // draw outline for this sprite
      if (this->show_outline_) {
         sf::RectangleShape s(sf::Vector2f((*sprite_it)->getGlobalBounds().width, (*sprite_it)->getGlobalBounds().height));      

         if (physics) {
            s.setPosition(physics->get_position());
         }
         s.setOutlineThickness(1);
         s.setOutlineColor(sf::Color::Blue);
         s.setFillColor(sf::Color::Transparent);

         viewport.draw(s);
      }
   }

   shape_list_t::iterator shape_it;
   for (shape_it = this->shapes_.begin(); shape_it != this->shapes_.end(); shape_it++) {
      if (physics) {
         (*shape_it)->setPosition(physics->get_position());
      }

      viewport.draw(*(*shape_it));
   }

   // draw diagnostic info
   if (physics && this->show_debug_text_) {
      sf::Vector2f pos = physics->get_position();
      sf::Vector2i map_idx;
      sf::RectangleShape bounding_box_graphic;
      
      // convert to map index (TODO: encapsulate this logic somewhere else...)
      map_idx = static_cast<sf::Vector2i>(viewport.get_world_coord(static_cast<sf::Vector2i>(pos)));

      map_idx.x = (int)(map_idx.x / (viewport.get_scale() * Settings::Instance()->TILE_WIDTH));
      map_idx.y = (int)(map_idx.y / (viewport.get_scale() * Settings::Instance()->TILE_HEIGHT));
   
      //viewport.write(std::to_string(map_idx.x) + ", " + std::to_string(map_idx.y), pos - sf::Vector2f(0, 11), &this->font_debug_);
      viewport.write(std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y), pos - sf::Vector2f(0, 11), &this->font_debug_);

      // draw physics bounding box
      bounding_box_graphic.setSize(physics->get_size());
      bounding_box_graphic.setPosition(physics->get_position());
      bounding_box_graphic.setFillColor(sf::Color::Transparent);
      bounding_box_graphic.setOutlineColor(sf::Color::Red); // change color depending on solidity
      bounding_box_graphic.setOutlineThickness(1.0);

      viewport.draw(bounding_box_graphic);
   }
}
