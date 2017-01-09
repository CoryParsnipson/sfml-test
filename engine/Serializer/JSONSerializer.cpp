#include "JSONSerializer.h"

JSONSerializer::JSONSerializer(int tab_width /* = 0 */)
: tab_width_(tab_width)
{
}

std::string JSONSerializer::serialize(Serializer::SerialData& s) {
   nlohmann::json data;

   Serializer::SerialData::const_iterator it;
   for (it = s.begin(); it != s.end(); ++it) {
      data[it->first] = it->second;
   }

   return data.dump(this->tab_width_);
}

Serializer::SerialData JSONSerializer::deserialize(Game& g, Channel& c) {
   std::string json_string;

   Serializer::SerialData serial_data;
   nlohmann::json data = json::parse(json_string);

   // TODO: implement
   // need to keep receiving from channel until an entire JSON object is obtained
   // keep excess of last receive
   
   for (json::const_iterator it = data.begin(); it != data.end(); ++it) {
      serial_data[it.key()] = it.value();
   }

   return serial_data;
}
