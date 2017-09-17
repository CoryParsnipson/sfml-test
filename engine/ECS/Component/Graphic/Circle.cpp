#include <algorithm>

#include "Circle.h"
#include "RenderSurface.h"

// ----------------------------------------------------------------------------
// Circle implementation
// ----------------------------------------------------------------------------
Circle::Circle(const std::string& id /* = Circle */, float radius /* = 10 */, float x /* = 0 */, float y /* = 0 */)
: Component(id)
, drawable_(radius)
{
   this->drawable_.setPosition(x, y);
}

Circle::Circle(const std::string& id, float radius /* = 10 */, sf::Vector2f pos /* = sf::Vector2f(0, 0) */)
: Component(id)
, drawable_(radius)
{
   this->drawable_.setPosition(pos);
}

Circle::Circle(const Circle& other)
: Component(other.id())
, drawable_(other.drawable_)
{
}

Circle::~Circle() {
}

Circle& Circle::operator=(const Circle& other) {
   Circle tmp(other);
   this->swap(tmp);
   return *this;
}

void Circle::swap(Circle& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->drawable_, other.drawable_);
}

void Circle::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(this->drawable_, render_states);
}

sf::FloatRect Circle::local_bounds() const {
   return this->drawable_.getLocalBounds();
}

sf::FloatRect Circle::global_bounds() const {
   return this->drawable_.getGlobalBounds();
}

void Circle::position(float x, float y) {
   this->drawable_.setPosition(x, y);
}

const sf::Vector2f& Circle::position() const {
   return this->drawable_.getPosition();
}

void Circle::move(float x, float y) {
   this->drawable_.move(x, y);
}

void Circle::rotation(float angle) {
   this->drawable_.setRotation(angle);
}

float Circle::rotation() const {
   return this->drawable_.getRotation();
}

void Circle::scale(float x, float y) {
   this->drawable_.setScale(x, y);
}

const sf::Vector2f& Circle::scale() const {
   return this->drawable_.getScale();
}

void Circle::origin(float x, float y) {
   this->drawable_.setOrigin(x, y);
}

const sf::Vector2f& Circle::origin() const {
   return this->drawable_.getOrigin();
}

void Circle::color(const sf::Color& color) {
   this->drawable_.setFillColor(color);
}

const sf::Color& Circle::color() const {
   return this->drawable_.getFillColor();
}

const sf::Transform& Circle::transform() const {
   return this->drawable_.getTransform();
}

void Circle::radius(float radius) {
   this->drawable_.setRadius(radius);
}

float Circle::radius() const {
   return this->drawable_.getRadius();
}

void Circle::outline_color(const sf::Color& color) {
   this->drawable_.setOutlineColor(color);
}

const sf::Color& Circle::outline_color() const {
   return this->drawable_.getOutlineColor();
}

void Circle::outline_thickness(float thickness) {
   this->drawable_.setOutlineThickness(thickness);
}

float Circle::outline_thickness() const {
   return this->drawable_.getOutlineThickness();
}

std::string Circle::serialize(Serializer& s) {
   Serializer::SerialData data;

   int color = 0;
   int outline_color = 0;

   color = this->color().r;
   color |= this->color().g << 8;
   color |= this->color().b << 16;
   color |= this->color().a << 24;

   outline_color = this->outline_color().r;
   outline_color |= this->outline_color().g << 8;
   outline_color |= this->outline_color().b << 16;
   outline_color |= this->outline_color().a << 24;

   data["type"] = "Circle";
   
   data["id"] = this->id();
   data["x"] = std::to_string(this->position().x);
   data["y"] = std::to_string(this->position().y);
   data["rotation"] = std::to_string(this->rotation());
   data["origin_x"] = std::to_string(this->origin().x);
   data["origin_y"] = std::to_string(this->origin().y);
   data["color"] = std::to_string(color);
   data["radius"] = std::to_string(this->radius());
   data["outline_color"] = std::to_string(outline_color);
   data["outline_thickness"] = std::to_string(this->outline_thickness());

   return s.serialize(data);
}

void Circle::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   int raw_color = std::stoi(data["color"]);
   int raw_outline_color = std::stoi(data["outline_color"]);

   int color_r = raw_color & 0xFF;
   int color_g = (raw_color & 0xFF00) >> 8;
   int color_b = (raw_color & 0xFF0000) >> 16;
   int color_a = (raw_color & 0xFF000000) >> 24;

   int outline_color_r = raw_outline_color & 0xFF;
   int outline_color_g = (raw_outline_color & 0xFF00) >> 8;
   int outline_color_b = (raw_outline_color & 0xFF0000) >> 16;
   int outline_color_a = (raw_outline_color & 0xFF000000) >> 24;

   sf::Color color(color_r, color_g, color_b, color_a);
   sf::Color outline_color(outline_color_r, outline_color_g, outline_color_b, outline_color_a);

   this->id(data["id"]);
   this->position(std::stof(data["x"]), std::stof(data["y"]));
   this->rotation(std::stof(data["rotation"]));
   this->origin(std::stof(data["origin_x"]), std::stof(data["origin_y"]));
   this->color(color);
   this->radius(std::stof(data["radius"]));
   this->outline_color(outline_color);
   this->outline_thickness(std::stof(data["outline_thickness"]));
}
