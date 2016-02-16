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

bool GraphicsPart::get_show_outline() {
   return this->show_outline_;
}

void GraphicsPart::draw(Graphics& graphics, Layer& layer) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->draw(graphics, layer);

      sf::FloatRect bounds_rect = (*sprite_it)->get_global_bounds();

      // draw outline for this sprite
      if (this->show_outline_) {
         sf::RectangleShape bounds;
         
         bounds.setPosition(sf::Vector2f(bounds_rect.left, bounds_rect.top));
         bounds.setSize(sf::Vector2f(bounds_rect.width, bounds_rect.height));

         bounds.setFillColor(sf::Color::Transparent);
         bounds.setOutlineColor(sf::Color::Blue);
         bounds.setOutlineThickness(1.0);

         graphics.draw(bounds, layer);
      }

      // draw diagnostic info
      if (this->show_debug_text_) {
         sf::Vector2f global_pos(bounds_rect.left, bounds_rect.top);

         Text* debug_text = TextFactory::inst()->create_text(
            std::to_string((int)global_pos.x) + ", " + std::to_string((int)global_pos.y),
            "retro", 
            global_pos - sf::Vector2f(0, 10)
         );
         debug_text->draw(graphics, layer);

         delete debug_text;
         debug_text = nullptr;
      }
   }
}

void GraphicsPart::update(Game& game, Scene* scene, Entity* entity) {
   if (!entity) {
      Service::get_logger().msg("GraphicsPart", Logger::ERROR, "Entity null pointer received.");
      return;
   }
}
