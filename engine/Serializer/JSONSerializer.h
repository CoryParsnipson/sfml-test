#ifndef JSON_SERIALIZER_H
#define JSON_SERIZLIZER_H

#include "json.hpp"

#include "Serializer.h"

// ----------------------------------------------------------------------------
// JSONSerializer
//
// This is a serializer that can output json format strings.
// ----------------------------------------------------------------------------
class JSONSerializer : public Serializer {
public:
   JSONSerializer(int tab_width = 0);

   virtual std::string serialize(Serializer::SerialData& s);
   virtual Serializer::SerialData deserialize(Game& g, std::string& d);

   virtual std::string read(Channel& c);

private:
   int tab_width_;
};

#endif
