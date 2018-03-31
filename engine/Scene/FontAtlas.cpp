#include "FontAtlas.h"
#include "Font.h"
#include "Game.h"

FontAtlas::FontAtlas()
: Atlas("FontAtlas")
{
}

void FontAtlas::load(std::string font_key, std::string font_filename) {
   this->set(font_key, new Font(font_key, font_filename));
}

void FontAtlas::unload(std::string font_key) {
   this->set(font_key, nullptr);
}

std::string FontAtlas::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["id"] = this->id();
   for (ItemList::iterator it = this->begin(); it != this->end(); ++it) {
      data[it->first] = it->second->serialize(s);
   }

   return s.serialize(data);
}

void FontAtlas::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   this->id(data["id"]);

   data.erase(data.find("id"));
   for (Serializer::SerialData::iterator it = data.begin(); it != data.end(); ++it) {
      Font* f = new Font(it->first, "");
      f->deserialize(s, scene, it->second);

      this->set(it->first, f);
   }
}
