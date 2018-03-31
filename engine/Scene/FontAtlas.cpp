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
