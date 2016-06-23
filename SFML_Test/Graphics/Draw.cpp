#include "Draw.h"

#include "Texture.h"
#include "RenderSurface.h"

SpriteGraphic::SpriteGraphic()
: drawable_(new sf::RectangleShape()) {
}

SpriteGraphic::SpriteGraphic(Texture& texture)
: drawable_(new sf::RectangleShape()) {
   this->set_texture(texture);
}

SpriteGraphic::~SpriteGraphic() {
   delete this->drawable_;
}

void SpriteGraphic::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(*this->drawable_, render_states);

   // combine supplied render states with current render states
   render_states.transform = render_states.transform * this->drawable_->getTransform();

   Graphic::draw(surface, render_states);
}

sf::FloatRect SpriteGraphic::get_local_bounds() const {
   return this->drawable_->getLocalBounds();
}

sf::FloatRect SpriteGraphic::get_global_bounds() const {
   return this->drawable_->getGlobalBounds();
}

void SpriteGraphic::set_position(float x, float y) {
   this->drawable_->setPosition(sf::Vector2f(x, y));
}

void SpriteGraphic::set_position(const sf::Vector2f& pos) {
   this->drawable_->setPosition(pos);
}

void SpriteGraphic::set_size(float x, float y) {
   this->drawable_->setSize(sf::Vector2f(x, y));
}

void SpriteGraphic::set_size(const sf::Vector2f& size) {
   this->drawable_->setSize(size);
}

void SpriteGraphic::set_rotation(float angle) {
   this->drawable_->setRotation(angle);
}

void SpriteGraphic::set_scale(float factorX, float factorY) {
   this->drawable_->setScale(sf::Vector2f(factorX, factorY));
}

void SpriteGraphic::set_scale(const sf::Vector2f& factors) {
   this->drawable_->setScale(factors);
}

void SpriteGraphic::set_origin(float x, float y) {
   this->drawable_->setOrigin(sf::Vector2f(x, y));
}

void SpriteGraphic::set_origin(const sf::Vector2f& origin) {
   this->drawable_->setOrigin(origin);
}

void SpriteGraphic::set_color(const sf::Color& color) {
   this->drawable_->setFillColor(color);
}

const sf::Vector2f& SpriteGraphic::get_position() {
   return this->drawable_->getPosition();
}

sf::Vector2f SpriteGraphic::get_size() const {
   sf::Vector2f effective_size = this->drawable_->getSize();
   sf::Vector2f scale = this->get_scale();

   effective_size.x *= scale.x;
   effective_size.y *= scale.y;

   return effective_size;
}

const sf::Vector2f& SpriteGraphic::get_scale() const {
   return this->drawable_->getScale();
}

const sf::Vector2f& SpriteGraphic::get_origin() const {
   return this->drawable_->getOrigin();
}

const sf::Color& SpriteGraphic::get_color() const {
   return this->drawable_->getFillColor();
}

float SpriteGraphic::get_rotation() const {
   return this->drawable_->getRotation();
}

void SpriteGraphic::move(float offsetX, float offsetY) {
   return this->drawable_->move(sf::Vector2f(offsetX, offsetY));
}

void SpriteGraphic::move(const sf::Vector2f& offset) {
   return this->drawable_->move(offset);
}

void SpriteGraphic::rotate(float angle) {
   return this->drawable_->rotate(angle);
}

void SpriteGraphic::scale(float factorX, float factorY) {
   return this->drawable_->scale(sf::Vector2f(factorX, factorY));
}

void SpriteGraphic::scale(const sf::Vector2f& factor) {
   return this->drawable_->scale(factor);
}

const sf::Transform& SpriteGraphic::transform() const {
   return this->drawable_->getTransform();
}

void SpriteGraphic::set_texture(Texture& texture) {
   this->texture_ = &texture;
   this->drawable_->setTexture(&texture.get_texture(), true);
   this->set_size(texture.get_size());
}

void SpriteGraphic::set_texture_rect(const sf::IntRect& rect) {
   this->drawable_->setTextureRect(rect);
}

void SpriteGraphic::set_outline_color(const sf::Color& color) {
   this->drawable_->setOutlineColor(color);
}

void SpriteGraphic::set_outline_thickness(float thickness) {
   this->drawable_->setOutlineThickness(thickness);
}

Texture* SpriteGraphic::get_texture() const {
   return this->texture_;
}

const sf::IntRect& SpriteGraphic::get_texture_rect() const {
   return this->drawable_->getTextureRect();
}

const sf::Color& SpriteGraphic::get_outline_color() const {
   return this->drawable_->getOutlineColor();
}

float SpriteGraphic::get_outline_thickness() const {
   return this->drawable_->getOutlineThickness();
}

void SpriteGraphic::set_string(const std::string& s) {}
void SpriteGraphic::set_font(sf::Font* font) {}
void SpriteGraphic::set_character_size(unsigned int size) {}
void SpriteGraphic::set_style(sf::Text::Style style) {}

std::string SpriteGraphic::get_string() { return ""; }
const sf::Font* SpriteGraphic::get_font() { return nullptr; }
unsigned int SpriteGraphic::get_character_size() const { return 0; }
unsigned int SpriteGraphic::get_em_width() const { return 0; }
sf::Vector2f SpriteGraphic::find_character_pos(std::size_t index) const { return sf::Vector2f(0, 0); }

TextGraphic::TextGraphic()
: drawable_(new sf::Text()) {
}

TextGraphic::TextGraphic(const char* text, sf::Font* font, unsigned int size)
: em_(new sf::Text())
, drawable_(new sf::Text()) {
   this->em_->setString("m");

   this->set_string(text);
   this->set_font(font);
   this->set_character_size(size);
}

TextGraphic::TextGraphic(const std::string& text, sf::Font* font, unsigned int size)
: em_(new sf::Text())
, drawable_(new sf::Text()) {
   this->em_->setString("m");

   this->set_string(text);
   this->set_font(font);
   this->set_character_size(size);
}

TextGraphic::~TextGraphic() {
   delete this->em_;
   delete this->drawable_;
}

void TextGraphic::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(*this->drawable_, render_states);

   // combine supplied render states with current render states
   render_states.transform = render_states.transform * this->drawable_->getTransform();

   Graphic::draw(surface, render_states);
}

sf::FloatRect TextGraphic::get_local_bounds() const {
   return this->drawable_->getLocalBounds();
}

sf::FloatRect TextGraphic::get_global_bounds() const {
   return this->drawable_->getGlobalBounds();
}

void TextGraphic::set_position(float x, float y) {
   this->drawable_->setPosition(sf::Vector2f(x, y));
}

void TextGraphic::set_position(const sf::Vector2f& pos) {
   this->drawable_->setPosition(pos);
}

void TextGraphic::set_size(float x, float y) {}
void TextGraphic::set_size(const sf::Vector2f& size) {}

void TextGraphic::set_rotation(float angle) {
   this->drawable_->setRotation(angle);
}

void TextGraphic::set_scale(float factorX, float factorY) {
   this->drawable_->setScale(sf::Vector2f(factorX, factorY));
}

void TextGraphic::set_scale(const sf::Vector2f& factors) {
   this->drawable_->setScale(factors);
}

void TextGraphic::set_origin(float x, float y) {
   this->drawable_->setOrigin(sf::Vector2f(x, y));
}

void TextGraphic::set_origin(const sf::Vector2f& origin) {
   this->drawable_->setOrigin(origin);
}

void TextGraphic::set_color(const sf::Color& color) {
   this->drawable_->setColor(color);
}

const sf::Vector2f& TextGraphic::get_position() {
   sf::FloatRect bounds = this->get_global_bounds();

   this->bounds_pos_.x = bounds.left;
   this->bounds_pos_.y = bounds.top;

   return this->bounds_pos_;
}

sf::Vector2f TextGraphic::get_size() const {
   return this->size_;
}

const sf::Vector2f& TextGraphic::get_scale() const {
   return this->drawable_->getScale();
}

const sf::Vector2f& TextGraphic::get_origin() const {
   return this->drawable_->getOrigin();
}

const sf::Color& TextGraphic::get_color() const {
   return this->drawable_->getColor();
}

float TextGraphic::get_rotation() const {
   return this->drawable_->getRotation();
}

void TextGraphic::move(float offsetX, float offsetY) {
   return this->drawable_->move(sf::Vector2f(offsetX, offsetY));
}

void TextGraphic::move(const sf::Vector2f& offset) {
   return this->drawable_->move(offset);
}

void TextGraphic::rotate(float angle) {
   this->drawable_->rotate(angle);

   this->size_.x = this->get_local_bounds().width;
   this->size_.y = this->get_local_bounds().height;
}

void TextGraphic::scale(float factorX, float factorY) {
   this->drawable_->scale(sf::Vector2f(factorX, factorY));

   this->size_.x = this->get_local_bounds().width;
   this->size_.y = this->get_local_bounds().height;
}

void TextGraphic::scale(const sf::Vector2f& factor) {
   this->drawable_->scale(factor);

   this->size_.x = this->get_local_bounds().width;
   this->size_.y = this->get_local_bounds().height;
}

const sf::Transform& TextGraphic::transform() const {
   return this->drawable_->getTransform();
}

void TextGraphic::set_texture(Texture& texture) {}
void TextGraphic::set_texture_rect(const sf::IntRect& rect) {}
void TextGraphic::set_outline_color(const sf::Color& color) {}
void TextGraphic::set_outline_thickness(float thickness) {}
Texture* TextGraphic::get_texture() const { return nullptr; }
const sf::IntRect& TextGraphic::get_texture_rect() const { return this->null_rect_; }
const sf::Color& TextGraphic::get_outline_color() const { return sf::Color::Transparent; }
float TextGraphic::get_outline_thickness() const { return 0.f; }

void TextGraphic::set_string(const std::string& s) {
   this->drawable_->setString(s);

   this->size_.x = this->get_local_bounds().width;
   this->size_.y = this->get_local_bounds().height;
}

void TextGraphic::set_font(sf::Font* font) {
   if (!font) {
      return;
   }
   this->em_->setFont(*font);
   this->drawable_->setFont(*font);

   this->size_.x = this->get_local_bounds().width;
   this->size_.y = this->get_local_bounds().height;
}

void TextGraphic::set_character_size(unsigned int size) {
   this->em_->setCharacterSize(size);
   this->drawable_->setCharacterSize(size);

   this->size_.x = this->get_local_bounds().width;
   this->size_.y = this->get_local_bounds().height;
}

void TextGraphic::set_style(sf::Text::Style style) {
   this->em_->setStyle(style);
   this->drawable_->setStyle(style);

   this->size_.x = this->get_local_bounds().width;
   this->size_.y = this->get_local_bounds().height;
}

std::string TextGraphic::get_string() {
   return this->drawable_->getString();
}

const sf::Font* TextGraphic::get_font() {
   return this->drawable_->getFont();
}

unsigned int TextGraphic::get_character_size() const {
   return this->drawable_->getCharacterSize();
}

unsigned int TextGraphic::get_em_width() const {
   return this->em_->getLocalBounds().width;
}

sf::Vector2f TextGraphic::find_character_pos(std::size_t index) const {
   return this->drawable_->findCharacterPos(index);
}

VertexGraphic::VertexGraphic(sf::VertexArray* arr)
: state_(sf::RenderStates::Default)
, nullrect_(0, 0, 0, 0)
, drawable_(arr)
{
   this->size_.x = this->drawable_->getBounds().width;
   this->size_.y = this->drawable_->getBounds().height;
}

VertexGraphic::~VertexGraphic() {
   delete this->drawable_;
}

void VertexGraphic::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   // combine supplied render states with current render states
   render_states.transform = render_states.transform * this->state_.transform;
   surface.draw(*this->drawable_, render_states);
   Graphic::draw(surface, render_states);
}

sf::FloatRect VertexGraphic::get_local_bounds() const {
   return this->drawable_->getBounds();
}

sf::FloatRect VertexGraphic::get_global_bounds() const {
   return this->drawable_->getBounds();
}

void VertexGraphic::set_size(float x, float y) {
   (*this->drawable_)[1].position = sf::Vector2f(0, y);
   (*this->drawable_)[2].position = sf::Vector2f(x, 0);
   (*this->drawable_)[3].position = sf::Vector2f(x, y);
}

void VertexGraphic::set_size(const sf::Vector2f& size) {
}

const sf::Vector2f& VertexGraphic::get_position() {
   this->pos_.x = this->drawable_->getBounds().left;
   this->pos_.y = this->drawable_->getBounds().top;

   this->pos_ = this->state_.transform.transformPoint(this->pos_);
   return this->pos_;
}

sf::Vector2f VertexGraphic::get_size() const {
   return this->size_;
}

const sf::Vector2f& VertexGraphic::get_scale() const {
   return this->scale_;
}

const sf::Vector2f& VertexGraphic::get_origin() const {
   return this->pos_;
}

const sf::Color& VertexGraphic::get_color() const {
   return sf::Color::Transparent;
}

float VertexGraphic::get_rotation() const {
   return 0.0;
}

void VertexGraphic::move(float offsetX, float offsetY) {
   this->move(sf::Vector2f(offsetX, offsetY));
}

void VertexGraphic::move(const sf::Vector2f& offset) {
   this->state_.transform.translate(offset);
}

void VertexGraphic::rotate(float angle) {
   this->state_.transform.rotate(angle);
}

void VertexGraphic::scale(float factorX, float factorY) {
   this->scale_.x *= factorX;
   this->scale_.y *= factorY;

   this->size_.x = this->drawable_->getBounds().width;
   this->size_.y = this->drawable_->getBounds().height;

   this->state_.transform.scale(factorX, factorY);
}

void VertexGraphic::scale(const sf::Vector2f& factor) {
   this->scale_.x *= factor.x;
   this->scale_.y *= factor.y;

   this->size_.x = this->drawable_->getBounds().width;
   this->size_.y = this->drawable_->getBounds().height;

   this->state_.transform.scale(factor);
}

const sf::Transform& VertexGraphic::transform() const {
   return this->state_.transform;
}

Texture* VertexGraphic::get_texture() const {
   return nullptr;
}

const sf::IntRect& VertexGraphic::get_texture_rect() const {
   return this->nullrect_;
}

const sf::Color& VertexGraphic::get_outline_color() const {
   return sf::Color::Transparent;
}

float VertexGraphic::get_outline_thickness() const {
   return 0.f;
}

std::string VertexGraphic::get_string() {
   return "";
}

const sf::Font* VertexGraphic::get_font() {
   return nullptr;
}

unsigned int VertexGraphic::get_character_size() const {
   return 0;
}

unsigned int VertexGraphic::get_em_width() const {
   return 0;
}

sf::Vector2f VertexGraphic::find_character_pos(std::size_t index) const {
   return sf::Vector2f(0, 0);
}
