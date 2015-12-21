#ifndef UTIL_FACTORY_H
#define UTIL_FACTORY_H

#include "Entity2.h"

class UtilFactory {
public:
   static UtilFactory* inst();
   virtual ~UtilFactory();

   virtual Entity2* create_mouse();

protected:
   // seal off constructors
   UtilFactory();
   UtilFactory(UtilFactory& f);

   static UtilFactory* inst_;
};

#endif
