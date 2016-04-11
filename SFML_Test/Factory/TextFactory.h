#ifndef TEXT_FACTORY_H
#define TEXT_FACTORY_H

#include "dependencies.h"

class Text;
class Entity;
class TextGraphic;

class TextFactory {
public:
   typedef std::map<std::string, sf::Font*> FontList;

   enum ALIGN {
      LEFT,
      RIGHT,
      CENTER
   };

   static TextFactory* inst();
   virtual ~TextFactory();

   // font management interface
   void load_font(std::string font_name, std::string filename);
   void unload_font(std::string font_name);
   sf::Font* get_font(std::string font_name);

   // factory methods
   virtual TextGraphic* create_text(
      std::string msg,
      sf::Font* font,
      sf::Vector2f pos = sf::Vector2f(0, 0),
      int size = 12,
      ALIGN alignment = ALIGN::LEFT,
      sf::Color color = sf::Color::White,
      int layer = 0
   );

   virtual TextGraphic* create_text(
      std::string msg,
      std::string font,
      sf::Vector2f pos = sf::Vector2f(0, 0),
      int size = 12,
      ALIGN alignment = ALIGN::LEFT,
      sf::Color color = sf::Color::White,
      int layer = 0
   );

   virtual Entity* create_text_entity(
      std::string msg,
      std::string font,
      sf::Vector2f pos = sf::Vector2f(0, 0),
      int size = 12,
      ALIGN alignment = ALIGN::LEFT,
      sf::Color color = sf::Color::White,
      int layer = 0,
      bool debug = false
   );

protected:
   static TextFactory* inst_;

   // seal off constructors
   TextFactory();
   TextFactory(TextFactory& f);

   sf::Vector2f get_origin_from_alignment(const sf::FloatRect& bounds, ALIGN alignment);

   FontList fonts_;
};

#endif
