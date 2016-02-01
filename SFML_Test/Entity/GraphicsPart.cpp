#include "GraphicsPart.h"

#include "Graphics.h"
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

   ShapeList::const_iterator shape_it;
   for (shape_it = this->shapes_.begin(); shape_it != this->shapes_.end(); ++shape_it) {
      delete *shape_it;
   }
   this->shapes_.clear();

   TextList::const_iterator text_it;
   for (text_it = this->texts_.begin(); text_it != this->texts_.end(); ++text_it) {
      delete *text_it;
   }
   this->texts_.clear();
}

void GraphicsPart::add(sf::Sprite* sprite) {
   this->sprites_.push_back(sprite);
}

void GraphicsPart::add(sf::Shape* shape) {
   this->shapes_.push_back(shape);
}

void GraphicsPart::add(sf::Text* text) {
   this->texts_.push_back(text);
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
      graphics.draw(*(*sprite_it), layer.get_view());

      // draw outline for this sprite
      if (this->show_outline_) {
         sf::RectangleShape s(sf::Vector2f((*sprite_it)->getGlobalBounds().width, (*sprite_it)->getGlobalBounds().height));      

         s.setPosition((*sprite_it)->getPosition());
         s.setOutlineThickness(1);
         s.setOutlineColor(sf::Color::Blue);
         s.setFillColor(sf::Color::Transparent);

         graphics.draw(s, layer.get_view());
      }
   }

   ShapeList::const_iterator shape_it;
   for (shape_it = this->shapes_.begin(); shape_it != this->shapes_.end(); ++shape_it) {
      graphics.draw(*(*shape_it), layer.get_view());
   }

   sf::Vector2f pos(0, 0);
   sf::Vector2f size(0, 0);
   TextList::const_iterator text_it;
   for (text_it = this->texts_.begin(); text_it != this->texts_.end(); ++text_it) {
      pos = (*text_it)->getPosition();

      size.x = (*text_it)->getGlobalBounds().width;
      size.y = (*text_it)->getGlobalBounds().height;

      graphics.draw(*(*text_it), layer.get_view());
   }

   // draw diagnostic info
   if (this->show_debug_text_) {
      sf::Vector2i map_idx;
      sf::RectangleShape bounding_box_graphic;
      
      // convert to map index (TODO: encapsulate this logic somewhere else...)
      //map_idx = static_cast<sf::Vector2i>(viewport.get_world_coord(static_cast<sf::Vector2i>(pos)));

      //map_idx.x = (int)(map_idx.x / (viewport.get_scale() * Settings::Instance()->TILE_WIDTH));
      //map_idx.y = (int)(map_idx.y / (viewport.get_scale() * Settings::Instance()->TILE_HEIGHT));
   
      //viewport.write(std::to_string(map_idx.x) + ", " + std::to_string(map_idx.y), pos - sf::Vector2f(0, 11), &this->font_debug_);
      //viewport.write(std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y), pos - sf::Vector2f(0, 11), &this->font_debug_);

      // draw physics bounding box
      bounding_box_graphic.setPosition(pos);
      bounding_box_graphic.setFillColor(sf::Color::Transparent);
      bounding_box_graphic.setOutlineColor(sf::Color::Red); // change color depending on solidity
      bounding_box_graphic.setOutlineThickness(1.0);

      graphics.draw(bounding_box_graphic, layer.get_view());
   }
}

void GraphicsPart::update(Game& game, Scene* scene, Entity* entity) {
   if (!entity) {
      Service::get_logger().msg("GraphicsPart", Logger::ERROR, "Entity null pointer received.");
      return;
   }
   
   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity->get("physics"));
   if (!physics) {
      return;
   }
   
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); sprite_it++) {
      // update graphics draw location based on physical position of entity
      (*sprite_it)->setPosition(physics->get_position());
   }

   ShapeList::const_iterator shape_it;
   for (shape_it = this->shapes_.begin(); shape_it != this->shapes_.end(); shape_it++) {
      (*shape_it)->setPosition(physics->get_position());
   }

   TextList::const_iterator text_it;
   for (text_it = this->texts_.begin(); text_it != this->texts_.end(); ++text_it) {
      (*text_it)->setPosition(physics->get_position());
   }
}
