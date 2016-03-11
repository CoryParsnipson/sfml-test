#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "dependencies.h"

class Serialize {
public:
   typedef std::map<std::string, std::string> SerialObj;

   virtual SerialObj serialize() = 0;

   // should throw out of range exception on problem
   virtual void deserialize(SerialObj& obj) = 0;
};

#endif
