#ifndef UTIL_FACTORY_H
#define UTIL_FACTORY_H

#include "Entity.h"

class Graphic;

class UtilFactory {
public:
   static UtilFactory* inst();
   virtual ~UtilFactory();

   virtual Entity* create_mouse();
   
   virtual Entity* create_graphic(Graphic* g, sf::FloatRect bounding_box);

   virtual sf::FloatRect* create_float_rect(sf::Vector2f& one, sf::Vector2f& two);

protected:
   // seal off constructors
   UtilFactory();
   UtilFactory(UtilFactory& f);

   static UtilFactory* inst_;
};

#endif
