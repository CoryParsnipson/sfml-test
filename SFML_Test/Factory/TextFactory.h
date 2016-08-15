#ifndef TEXT_FACTORY_H
#define TEXT_FACTORY_H

#include "dependencies.h"

class Text;
class Entity;
class TextGraphic;

class TextFactory {
public:
   enum ALIGN {
      LEFT,
      RIGHT,
      CENTER
   };

   static TextFactory* inst();
   virtual ~TextFactory();

   // factory methods
   virtual TextGraphic* create_text(
      std::string msg,
      sf::Font* font,
      sf::Vector2f pos = sf::Vector2f(0, 0),
      int size = 12,
      ALIGN alignment = ALIGN::LEFT,
      sf::Color color = sf::Color::White
   );

   virtual Entity* create_text_entity(
      std::string msg,
      sf::Font* font,
      sf::Vector2f pos = sf::Vector2f(0, 0),
      int size = 12,
      ALIGN alignment = ALIGN::LEFT,
      sf::Color color = sf::Color::White,
      bool debug = false
   );

protected:
   static TextFactory* inst_;

   // seal off constructors
   TextFactory();
   TextFactory(TextFactory& f);

   sf::Vector2f get_origin_from_alignment(const sf::FloatRect& bounds, ALIGN alignment);
};

#endif
