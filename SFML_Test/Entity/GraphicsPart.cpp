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
}

void GraphicsPart::add(sf::Drawable* sprite) {
   this->sprites_.push_back(sprite);
}

sf::Drawable* GraphicsPart::get(int idx) {
   sf::Drawable* sprite = nullptr;

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
   sf::Vector2f pos(0, 0);
   sf::Vector2f size(0, 0);

   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      // TODO: okay, do this better. definitely do this better
      // For some reason, getGlobalBounds is not in the sf::Transformable interface, but
      // it is present on all its child classes
      sf::Sprite* sp = dynamic_cast<sf::Sprite*>(*sprite_it);
      sf::Shape* sh = dynamic_cast<sf::Shape*>(*sprite_it);
      sf::Text* te = dynamic_cast<sf::Text*>(*sprite_it);

      if (sp) {
         pos.x = sp->getGlobalBounds().left;
         pos.y = sp->getGlobalBounds().top;

         size.x = sp->getGlobalBounds().width;
         size.y = sp->getGlobalBounds().height;
      }

      if (sh) {
         pos.x = sh->getGlobalBounds().left;
         pos.y = sh->getGlobalBounds().top;

         size.x = sh->getGlobalBounds().width;
         size.y = sh->getGlobalBounds().height;
      }

      if (te) {
         pos.x = te->getGlobalBounds().left;
         pos.y = te->getGlobalBounds().top;

         size.x = te->getGlobalBounds().width;
         size.y = te->getGlobalBounds().height;
      }

      // convert pos to world coordinates
      pos = graphics.get_world_coord(static_cast<sf::Vector2i>(pos), &layer.get_view());
      
      graphics.draw(*(*sprite_it), layer);

      // draw outline for this sprite
      if (this->show_outline_) {
         sf::RectangleShape s(size);

         s.setPosition(pos);
         s.setOutlineThickness(1);
         s.setOutlineColor(sf::Color::Blue);
         s.setFillColor(sf::Color::Transparent);

         graphics.draw(s, layer);
      }
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
      bounding_box_graphic.setSize(size);
      bounding_box_graphic.setFillColor(sf::Color::Transparent);
      bounding_box_graphic.setOutlineColor(sf::Color::Red); // change color depending on solidity
      bounding_box_graphic.setOutlineThickness(1.0);

      graphics.draw(bounding_box_graphic, layer);
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
      
      // TODO: UGGGGHHHHH
      sf::Sprite* sp = dynamic_cast<sf::Sprite*>(*sprite_it);
      sf::Shape* sh = dynamic_cast<sf::Shape*>(*sprite_it);
      sf::Text* te = dynamic_cast<sf::Text*>(*sprite_it);

      if (sp) { sp->setPosition(physics->get_position()); }
      if (sh) { sh->setPosition(physics->get_position()); }
      if (te) { te->setPosition(physics->get_position()); }
   }
}
