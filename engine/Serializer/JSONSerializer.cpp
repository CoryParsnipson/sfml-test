#include <string>

#include "JSONSerializer.h"
#include "Channel.h"

JSONSerializer::JSONSerializer(int tab_width /* = -1 */, int read_chunk_size /* = 100 */)
: tab_width_(tab_width)
, read_chunk_size_(read_chunk_size)
{
}

JSONSerializer::~JSONSerializer() {
}

std::string JSONSerializer::serialize(Serializer::SerialData& s) {
   nlohmann::json data;

   for (Serializer::SerialData::iterator it = s.begin(); it != s.end(); ++it) {
      if (it->second[0] == '{') {
         data[it->first] = nlohmann::json::parse(it->second);
      } else {
         data[it->first] = it->second;
      }
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
   int paren_count = 0;
   int channel_empty = false;
   int close_paren_pos = -1;
   std::string data;
   std::string rx_chunk;

   // read channel until we encounter the first open "{" (or until it's empty)
   while (!channel_empty) {
      channel_empty = !c.receive(rx_chunk, this->read_chunk_size_);
      if (rx_chunk.find_first_of("{") != std::string::npos) {
         break;
      }
   }

   // now count parentheses until they match up or until channel runs out of data
   while (true) {
      for (int i = 0; i < (signed)rx_chunk.length(); ++i) {

         if (rx_chunk[i] == '{') {
            ++paren_count;
         } else if (rx_chunk[i] == '}' && paren_count > 0) {
            --paren_count;

            // break upon matching close parenthesis to avoid capturing multiple objects
            if (!paren_count) {
               close_paren_pos = i;
               break;
            }
         }
      }

      if (close_paren_pos != -1) {
         data += rx_chunk.substr(0, close_paren_pos + 1);
      } else {
         data += rx_chunk;
      }

      if (channel_empty || !paren_count) {
         break;
      }
      channel_empty = !c.receive(rx_chunk, this->read_chunk_size_);
   }

   // seek channel to right after position of closing parenthesis
   if (close_paren_pos != -1) {
      c.seek(close_paren_pos - rx_chunk.length() + 1, Channel::Offset::Current);
   }

   return (data.find_first_of("{") == std::string::npos ? "" : data.substr(data.find_first_of("{")));
}
