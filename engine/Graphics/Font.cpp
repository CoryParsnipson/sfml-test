#include "Font.h"
#include "Game.h"

Font::Font(const std::string& id, const std::string& filename)
: id_(id)
, filename_(filename)
, font_()
{
   this->load_from_file(filename);
}

Font::Font(const Font& other)
: id_(other.id_)
, filename_(other.filename_)
, font_(other.font_)
{
}

Font::~Font() {
}

Font& Font::operator=(const Font& other) {
   Font tmp(other);
   this->swap(tmp);
   return *this;
}

void Font::swap(Font& other) {
   std::swap(this->id_, other.id_);
   std::swap(this->filename_, other.filename_);
   std::swap(this->font_, other.font_);
}

const std::string& Font::id() const {
   return this->id_;
}

void Font::id(const std::string& id) {
   this->id_ = id;
}

bool Font::load_from_file(const std::string& filename) {
   bool result = this->font_.loadFromFile(filename);
   if (result) {
      this->filename_ = filename;
   } else {
      Game::logger().msg(this->id(), Logger::ERROR, "Cannot load font '" + filename + "'.");
   }

   return result;
}

const sf::Glyph& Font::glyph(sf::Uint32 code_point, unsigned int character_size, bool bold) const {
   return this->font_.getGlyph(code_point, character_size, bold);
}

int Font::kerning(sf::Uint32 first, sf::Uint32 second, unsigned int character_size) const {
   return this->font_.getKerning(first, second, character_size);
}

int Font::line_spacing(unsigned int character_size) const {
   return this->font_.getLineSpacing(character_size);
}

const sf::Texture& Font::texture(unsigned int character_size) const {
   return this->font_.getTexture(character_size);
}

const sf::Font& Font::get_font() const {
   return this->font_;
}

std::string Font::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["id"] = this->id_;
   data["filename"] = this->filename_;

   return s.serialize(data);
}

void Font::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   this->id(data["id"]);
   this->filename_ = data["filename"];

   this->load_from_file(data["filename"]);
}
