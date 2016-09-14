#include "TextFactory.h"

#include "Draw.h"
#include "Entity.h"
#include "GraphicsPart.h"
#include "DebugPart.h"

// initialize static members
TextFactory* TextFactory::inst_ = new TextFactory();

TextFactory* TextFactory::inst() {
   return TextFactory::inst_;
}

TextFactory::TextFactory() {
}

TextFactory::~TextFactory() {
}

TextGraphic* TextFactory::create_text(std::string msg, sf::Font* font, sf::Vector2f pos, int size, ALIGN alignment, sf::Color color) {
   TextGraphic* t = new TextGraphic(msg, font, size);

   t->set_color(color);
   t->set_origin(this->get_origin_from_alignment(t->get_local_bounds(), alignment));
   t->set_position(pos);

   return t;
}

Entity* TextFactory::create_text_entity(std::string msg, sf::Font* font, sf::Vector2f pos, int size, ALIGN alignment, sf::Color color, bool debug) {
   Entity* text_entity = new Entity();
   GraphicsPart* text_entity_graphics = new GraphicsPart();

   text_entity_graphics->add(this->create_text(msg, font, pos, size, alignment, color));
   text_entity->add(text_entity_graphics);

   if (debug) {
      text_entity->add(new DebugPart());
   }

   return text_entity;
}

sf::Vector2f TextFactory::get_origin_from_alignment(const sf::FloatRect& bounds, ALIGN alignment) {
   sf::Vector2f origin(0, 0);

   switch (alignment) {
   case (ALIGN::CENTER) :
      origin.x = bounds.left + bounds.width / 2.0f;
      origin.y = bounds.top + bounds.height / 2.0f;
      break;
   case (ALIGN::RIGHT) :
      origin.x = bounds.width;
      origin.y = bounds.top + bounds.height / 2.0f;
      break;
   default :
      origin.x = bounds.left;
      origin.y = bounds.top;
   }

   return origin;
}
