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
   JSONSerializer(int tab_width = -1, int read_chunk_size = 100);
   virtual ~JSONSerializer();

   virtual std::string serialize(Serializer::SerialData& s);
   virtual Serializer::SerialData deserialize(Scene& scene, std::string& d);

   virtual std::string read(Channel& c);

private:
   int tab_width_;
   int read_chunk_size_;
};

#endif
