#include <algorithm>

#include "Text.h"
#include "RenderSurface.h"

// ----------------------------------------------------------------------------
// Text implementation
// ----------------------------------------------------------------------------
Text::Text(const std::string& id /* = "Text" */, const std::string& text /* = "" */, sf::Font* font /* = nullptr */, unsigned int size /* = 12 */)
: Component(id)
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
, position_(other.position_)
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
   std::swap(this->position_, other.position_);
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

void Text::position(float x, float y) {
   this->drawable_.setPosition(x, y);

   this->position_.x = this->global_bounds().left;
   this->position_.y = this->global_bounds().top;
}

const sf::Vector2f& Text::position() const {
   return this->position_;
}

void Text::move(float x, float y) {
   this->drawable_.move(x, y);
}

void Text::rotation(float angle) {
   this->drawable_.setRotation(angle);
}

float Text::rotation() const {
   return this->drawable_.getRotation();
}

void Text::scale(float x, float y) {
   this->drawable_.setScale(x, y);

   this->position_.x = this->global_bounds().left;
   this->position_.y = this->global_bounds().top;
}

const sf::Vector2f& Text::scale() const {
   return this->drawable_.getScale();
}

void Text::origin(float x, float y) {
   this->drawable_.setOrigin(x, y);

   this->position_.x = this->global_bounds().left;
   this->position_.y = this->global_bounds().top;
}

const sf::Vector2f& Text::origin() const {
   return this->drawable_.getOrigin();
}

void Text::color(const sf::Color& color) {
   this->drawable_.setColor(color);
}

const sf::Color& Text::color() const {
   return this->drawable_.getColor();
}

const sf::Transform& Text::transform() const {
   return this->drawable_.getTransform();
}

void Text::string(const std::string& s) {
   this->string_ = s;
   this->drawable_.setString(s);

   this->position_.x = this->global_bounds().left;
   this->position_.y = this->global_bounds().top;
}

const std::string& Text::string() const {
   return this->string_;
}

void Text::font(sf::Font* font) {
   if (!font) {
      return;
   }
   this->drawable_.setFont(*font);

   this->position_.x = this->global_bounds().left;
   this->position_.y = this->global_bounds().top;
}

const sf::Font* Text::font() const {
   return this->drawable_.getFont();
}

void Text::font_size(unsigned int size) {
   this->drawable_.setCharacterSize(size);

   this->position_.x = this->global_bounds().left;
   this->position_.y = this->global_bounds().top;
}

unsigned int Text::font_size() const {
   return this->drawable_.getCharacterSize();
}

void Text::style(sf::Text::Style style) {
   this->drawable_.setStyle(style);

   this->position_.x = this->global_bounds().left;
   this->position_.y = this->global_bounds().top;
}

sf::Text::Style Text::style() const {
   return static_cast<sf::Text::Style>(this->drawable_.getStyle());
}

unsigned int Text::em_width() const {
   sf::Text em;
   em.setString("m");
   em.setCharacterSize(this->font_size());
   
   if (this->font()) {
      em.setFont(*this->font());
   }

   return em.getLocalBounds().width;
}
