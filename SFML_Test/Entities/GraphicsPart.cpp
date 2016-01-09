#include "GraphicsPart.h"
#include "FontConfig.h"

#include "Entity.h"
#include "PhysicsPart.h"

GraphicsPart::GraphicsPart(std::string id, Viewport* hud_viewport)
: Part(id)
, show_outline_(false)
, show_debug_text_(false)
, font_debug_("retro", 11, FontConfig::ALIGN::LEFT)
, hud_viewport(hud_viewport)
{
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Creating GraphicsPart");
}

GraphicsPart::~GraphicsPart() {
   this->sprites_.clear();
   this->shapes_.clear();
}

void GraphicsPart::add(sf::Sprite* sprite) {
   this->sprites_.push_back(sprite);
}

void GraphicsPart::add(sf::Shape* shape) {
   this->shapes_.push_back(shape);
}

void GraphicsPart::set_viewport(Viewport* v) {
   this->hud_viewport = v;
}

void GraphicsPart::update(Entity& entity, Viewport& viewport) {
   Viewport* final_viewport = this->hud_viewport ? this->hud_viewport : &viewport;
   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity.get("physics"));

   sprite_list_t::iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); sprite_it++) {
      // update graphics draw location based on physical position of entity
      if (physics) {
         (*sprite_it)->setPosition(physics->get_position());
      }

      final_viewport->draw(*(*sprite_it));

      // draw outline for this sprite
      if (this->show_outline_) {
         sf::RectangleShape s(sf::Vector2f((*sprite_it)->getGlobalBounds().width, (*sprite_it)->getGlobalBounds().height));      

         if (physics) {
            s.setPosition(physics->get_position());
         }
         s.setOutlineThickness(1);
         s.setOutlineColor(sf::Color::Red);
         s.setFillColor(sf::Color::Transparent);

         final_viewport->draw(s);
      }
   }

   shape_list_t::iterator shape_it;
   for (shape_it = this->shapes_.begin(); shape_it != this->shapes_.end(); shape_it++) {
      if (physics) {
         (*shape_it)->setPosition(physics->get_position());
      }

      final_viewport->draw(*(*shape_it));
   }

   // draw diagnostic info
   if (physics && this->show_debug_text_) {
      sf::Vector2f pos = physics->get_position();
      final_viewport->write(std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y), pos - sf::Vector2f(0, 11), &this->font_debug_);
   }
}
