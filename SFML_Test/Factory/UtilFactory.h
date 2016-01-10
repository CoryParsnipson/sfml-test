#ifndef UTIL_FACTORY_H
#define UTIL_FACTORY_H

#include "Entity.h"

// forward declarations
class Viewport;

class UtilFactory {
public:
   static UtilFactory* inst();
   virtual ~UtilFactory();

   virtual Entity* create_mouse(Viewport* graphics_viewport = nullptr, Viewport* modified_viewport = nullptr);

protected:
   // seal off constructors
   UtilFactory();
   UtilFactory(UtilFactory& f);

   static UtilFactory* inst_;
};

#endif
