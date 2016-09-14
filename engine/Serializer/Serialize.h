#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <string>

#include "TextureAtlas.h"

class Serialize {
public:
   typedef std::map<std::string, std::string> SerialObj;

   virtual SerialObj serialize() = 0;

   // should throw out of range exception on problem
   virtual void deserialize(SerialObj& obj, const TextureAtlas* textures = nullptr) = 0;
};

#endif
