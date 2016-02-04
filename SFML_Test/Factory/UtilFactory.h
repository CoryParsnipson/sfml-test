#ifndef UTIL_FACTORY_H
#define UTIL_FACTORY_H

#include "Entity.h"

class UtilFactory {
public:
   static UtilFactory* inst();
   virtual ~UtilFactory();

   virtual Entity* create_mouse();
   
   virtual Entity* create_graphic(sf::Drawable* d, sf::Vector2f* pos = nullptr);

protected:
   // seal off constructors
   UtilFactory();
   UtilFactory(UtilFactory& f);

   static UtilFactory* inst_;
};

#endif
