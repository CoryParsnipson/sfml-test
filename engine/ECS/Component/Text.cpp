#include <algorithm>

#include "Text.h"
#include "Scene.h"
#include "RenderSurface.h"

// ----------------------------------------------------------------------------
// Text implementation
// ----------------------------------------------------------------------------
Text::Text(const std::string& id /* = "Text" */, const std::string& text /* = "" */, std::shared_ptr<Font> font /* = nullptr */, unsigned int size /* = 12 */)
: Component(id)
, font_(font)
, drawable_()
{
   this->string(text);
   this->font(font);
   this->font_size(size);
}

Text::Text(const Text& other)
: Component(other.id())
, string_(other.string_)
, drawable_(other.drawable_)
, offset_(other.offset_)
{
}

Text::~Text() {
}

Text& Text::operator=(const Text& other) {
   Text tmp(other);
   this->swap(tmp);
   return *this;
}

void Text::swap(Text& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->string_, other.string_);
   std::swap(this->drawable_, other.drawable_);
   std::swap(this->offset_, other.offset_);
}

void Text::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(this->drawable_, render_states);
}

sf::FloatRect Text::local_bounds() const {
   return this->drawable_.getLocalBounds();
}

sf::FloatRect Text::global_bounds() const {
   return this->drawable_.getGlobalBounds();
}

void Text::rotation(float angle) {
   this->drawable_.setRotation(angle);
}

float Text::rotation() const {
   return this->drawable_.getRotation();
}

void Text::scale(float x, float y) {
   this->drawable_.setScale(x, y);

   this->offset_.x = this->global_bounds().left;
   this->offset_.y = this->global_bounds().top;
}

const sf::Vector2f& Text::scale() const {
   return this->drawable_.getScale();
}

void Text::origin(float x, float y) {
   this->drawable_.setOrigin(x, y);

   this->offset_.x = this->global_bounds().left;
   this->offset_.y = this->global_bounds().top;
}

const sf::Vector2f& Text::origin() const {
   return this->drawable_.getOrigin();
}

void Text::offset(float x, float y) {
   this->drawable_.setPosition(x, y);

   this->offset_.x = this->global_bounds().left;
   this->offset_.y = this->global_bounds().top;
}

const sf::Vector2f& Text::offset() const {
   return this->offset_;
}

void Text::move_offset(float x, float y) {
   this->drawable_.move(x, y);
}

void Text::color(const sf::Color& color) {
   this->drawable_.setFillColor(color);
}

const sf::Color& Text::color() const {
   return this->drawable_.getFillColor();
}

const sf::Transform& Text::transform() const {
   return this->drawable_.getTransform();
}

void Text::string(const std::string& s) {
   this->string_ = s;
   this->drawable_.setString(s);

   this->offset_.x = this->global_bounds().left;
   this->offset_.y = this->global_bounds().top;
}

const std::string& Text::string() const {
   return this->string_;
}

void Text::font(std::shared_ptr<Font> font) {
   if (!font) {
      return;
   }
   this->drawable_.setFont(font->get_font());

   this->offset_.x = this->global_bounds().left;
   this->offset_.y = this->global_bounds().top;
}

std::shared_ptr<Font> Text::font() const {
   return this->font_;
}

void Text::font_size(unsigned int size) {
   this->drawable_.setCharacterSize(size);

   this->offset_.x = this->global_bounds().left;
   this->offset_.y = this->global_bounds().top;
}

unsigned int Text::font_size() const {
   return this->drawable_.getCharacterSize();
}

void Text::style(sf::Text::Style style) {
   this->drawable_.setStyle(style);

   this->offset_.x = this->global_bounds().left;
   this->offset_.y = this->global_bounds().top;
}

sf::Text::Style Text::style() const {
   return static_cast<sf::Text::Style>(this->drawable_.getStyle());
}

unsigned int Text::em_width() const {
   sf::Text em;
   em.setString("m");
   em.setCharacterSize(this->font_size());
   
   if (this->font()) {
      em.setFont(this->font()->get_font());
   }

   return em.getLocalBounds().width;
}

std::string Text::serialize(Serializer& s) {
   Serializer::SerialData data;

   Color color(this->color());

   data["type"] = "Text";

   data["id"] = this->id();
   data["x"] = std::to_string(this->offset().x);
   data["y"] = std::to_string(this->offset().y);
   data["rotation"] = std::to_string(this->rotation());
   data["origin_x"] = std::to_string(this->origin().x);
   data["origin_y"] = std::to_string(this->origin().y);
   data["color"] = color.serialize(s);
   data["text"] = this->string();
   data["font"] = this->font_->serialize(s);
   data["font_size"] = std::to_string(this->font_size());
   data["style"] = std::to_string(static_cast<int>(this->style()));

   return s.serialize(data);
}

void Text::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   Color color(sf::Color::Black);
   color.deserialize(s, scene, data["color"]);

   Serializer::SerialData font_data = s.deserialize(scene, data["font"]);
   
   this->id(data["id"]);
   this->offset(std::stof(data["x"]), std::stof(data["y"]));
   this->rotation(std::stof(data["rotation"]));
   this->origin(std::stof(data["origin_x"]), std::stof(data["origin_y"]));
   this->color(color);
   this->string(data["text"]);
   this->font(scene.fonts().get(font_data["id"])); // get font from scene (FontAtlas does the actual deserialization)
   this->font_size(std::stoi(data["font_size"]));
   this->style(static_cast<sf::Text::Style>(std::stoi(data["style"])));
}
