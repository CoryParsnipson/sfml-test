#ifndef UTIL_FACTORY_H
#define UTIL_FACTORY_H

#include "Entity.h"

class Graphic;
class GraphicsPart;

class UtilFactory {
public:
   static UtilFactory* inst();
   virtual ~UtilFactory();

   virtual Entity* create_graphic(Graphic* g, sf::FloatRect bounding_box, bool debug = false);

   virtual sf::FloatRect* create_float_rect(sf::Vector2f& one, sf::Vector2f& two);

protected:
   // seal off constructors
   UtilFactory();
   UtilFactory(const UtilFactory& f) = delete;
   UtilFactory& operator=(UtilFactory other) = delete;

   static UtilFactory* inst_;
};

#endif
