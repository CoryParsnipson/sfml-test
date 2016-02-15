#include "GraphicsPart.h"

#include "Graphics.h"
#include "Graphic.h"
#include "Viewport.h"
#include "Layer.h"
#include "Entity.h"
#include "PhysicsPart.h"

GraphicsPart::GraphicsPart(std::string id)
: Part(id)
, show_outline_(false)
, show_debug_text_(false)
{
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Creating GraphicsPart");
}

GraphicsPart::~GraphicsPart() {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      delete *sprite_it;
   }
   this->sprites_.clear();
}

void GraphicsPart::add(Graphic* sprite) {
   this->sprites_.push_back(sprite);
}

Graphic* GraphicsPart::get(int idx) {
   Graphic* sprite = nullptr;

   if (idx < 0 || idx >= (signed)this->sprites_.size()) {
      return sprite;
   }

   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      if (idx == 0) {
         sprite = *sprite_it;
         break;
      }
      --idx;
   }

   return sprite;
}

void GraphicsPart::set_position(float x, float y) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->set_position(x, y);
   }
}

void GraphicsPart::set_position(const sf::Vector2f& pos) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->set_position(pos);
   }
}

void GraphicsPart::set_size(float width, float height) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->set_size(width, height);
   }
}

void GraphicsPart::set_size(const sf::Vector2f& size) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->set_size(size);
   }
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

void GraphicsPart::draw(Graphics& graphics, Layer& layer) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->draw(graphics, layer);

      // draw outline for this sprite
      if (this->show_outline_) {
         sf::RectangleShape s((*sprite_it)->get_size());

         s.setPosition((*sprite_it)->get_position());
         s.setOutlineThickness(1);
         s.setOutlineColor(sf::Color::Blue);
         s.setFillColor(sf::Color::Transparent);

         graphics.draw(s, layer);
      }
   }

   // draw diagnostic info
   //if (this->show_debug_text_) {
   //   sf::Vector2i map_idx;
   //   sf::RectangleShape bounding_box_graphic;
   //   
   //   // convert to map index (TODO: encapsulate this logic somewhere else...)
   //   //map_idx = static_cast<sf::Vector2i>(viewport.get_world_coord(static_cast<sf::Vector2i>(pos)));

   //   //map_idx.x = (int)(map_idx.x / (viewport.get_scale() * Settings::Instance()->TILE_WIDTH));
   //   //map_idx.y = (int)(map_idx.y / (viewport.get_scale() * Settings::Instance()->TILE_HEIGHT));
   //
   //   //viewport.write(std::to_string(map_idx.x) + ", " + std::to_string(map_idx.y), pos - sf::Vector2f(0, 11), &this->font_debug_);
   //   //viewport.write(std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y), pos - sf::Vector2f(0, 11), &this->font_debug_);

   //   // draw physics bounding box
   //   bounding_box_graphic.setPosition(pos);
   //   bounding_box_graphic.setSize(size);
   //   bounding_box_graphic.setFillColor(sf::Color::Transparent);
   //   bounding_box_graphic.setOutlineColor(sf::Color::Red); // change color depending on solidity
   //   bounding_box_graphic.setOutlineThickness(1.0);

   //   graphics.draw(bounding_box_graphic, layer);
   //}
}

void GraphicsPart::update(Game& game, Scene* scene, Entity* entity) {
   if (!entity) {
      Service::get_logger().msg("GraphicsPart", Logger::ERROR, "Entity null pointer received.");
      return;
   }
}
