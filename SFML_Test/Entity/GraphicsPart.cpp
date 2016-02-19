#include "GraphicsPart.h"

#include "Graphics.h"
#include "Graphic.h"
#include "Viewport.h"
#include "Layer.h"
#include "Entity.h"
#include "PhysicsPart.h"

GraphicsPart::GraphicsPart(std::string id)
: Part(id)
{
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Creating GraphicsPart");
}

GraphicsPart::~GraphicsPart() {
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Destroying GraphicsPart");

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

void GraphicsPart::draw(Graphics& graphics) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->draw(graphics);
   }
}

void GraphicsPart::layer(Layer* layer) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->layer(layer);
   }
}

void GraphicsPart::update(Game& game, Scene* scene, Entity* entity) {
   if (!entity) {
      Service::get_logger().msg("GraphicsPart", Logger::ERROR, "Entity null pointer received.");
      return;
   }
}
