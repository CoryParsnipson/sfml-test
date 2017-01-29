#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <map>
#include <string>

#include "Channel.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Scene;
class Channel;

// ----------------------------------------------------------------------------
// Serializer
//
// This is an abstract base classes for components 
// objects and output them to strings in a specific format (and vice versa)
// ----------------------------------------------------------------------------
class Serializer {
public:
   using SerialData = std::map<std::string, std::string>;
   
   Serializer() {}
   virtual ~Serializer() {}

   virtual std::string serialize(Serializer::SerialData& s) = 0;
   virtual Serializer::SerialData deserialize(Scene& scene, std::string& d) = 0;

   // this should use the channel to receive an entire valid string
   virtual std::string read(Channel& c) = 0;
};

#endif
