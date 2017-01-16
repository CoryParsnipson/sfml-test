#include "JSONSerializer.h"
#include "Channel.h"

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

Serializer::SerialData JSONSerializer::deserialize(Game& g, std::string& d) {
   Serializer::SerialData serial_data;
   nlohmann::json data = nlohmann::json::parse(d);

   for (nlohmann::json::const_iterator it = data.begin(); it != data.end(); ++it) {
      serial_data[it.key()] = it.value();
   }

   return serial_data;
}

std::string JSONSerializer::read(Channel& c) {
   // TODO: implement
   // keep reading lines from channel
   // count open and close parentheses and stop reading lines when the numbers match
   // seek channel to just after the last closing parenthesis
   
   // return string
   return "";
}
