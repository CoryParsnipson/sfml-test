#include <algorithm>

#include "Rectangle.h"
#include "RenderSurface.h"

// ----------------------------------------------------------------------------
// Rectangle implementation
// ----------------------------------------------------------------------------
Rectangle::Rectangle(const std::string& id /* = "Rectangle" */, float x /* = 0 */, float y /* = 0 */, float width /* = 100 */, float height /* = 100 */)
: Component(id)
, drawable_()
{
   this->drawable_.setSize(sf::Vector2f(width, height));
   this->drawable_.setPosition(x, y);
}

Rectangle::Rectangle(const std::string& id, sf::FloatRect rect /* = sf::FloatRect(0, 0, 100, 100) */)
: Component(id)
, drawable_()
{
   this->drawable_.setSize(sf::Vector2f(rect.width, rect.height));
   this->drawable_.setPosition(rect.left, rect.top);
}

Rectangle::Rectangle(const Rectangle& other)
: Component(other.id())
, drawable_(other.drawable_)
{
}

Rectangle::~Rectangle() {
}

Rectangle& Rectangle::operator=(const Rectangle& other) {
   Rectangle tmp(other);
   this->swap(tmp);
   return *this;
}

void Rectangle::swap(Rectangle& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->drawable_, other.drawable_);
}

void Rectangle::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(this->drawable_, render_states);
}

sf::FloatRect Rectangle::local_bounds() const {
   return this->drawable_.getLocalBounds();
}

sf::FloatRect Rectangle::global_bounds() const {
   return this->drawable_.getGlobalBounds();
}

void Rectangle::position(float x, float y) {
   this->drawable_.setPosition(x, y);
}

const sf::Vector2f& Rectangle::position() const {
   return this->drawable_.getPosition();
}

void Rectangle::move(float x, float y) {
   this->drawable_.move(x, y);
}

void Rectangle::rotation(float angle) {
   this->drawable_.setRotation(angle);
}

float Rectangle::rotation() const {
   return this->drawable_.getRotation();
}

void Rectangle::scale(float x, float y) {
   this->drawable_.setScale(x, y);
}

const sf::Vector2f& Rectangle::scale() const {
   return this->drawable_.getScale();
}

void Rectangle::origin(float x, float y) {
   this->drawable_.setOrigin(x, y);
}

const sf::Vector2f& Rectangle::origin() const {
   return this->drawable_.getOrigin();
}

void Rectangle::color(const sf::Color& color) {
   this->drawable_.setFillColor(color);
}

const sf::Color& Rectangle::color() const {
   return this->drawable_.getFillColor();
}

const sf::Transform& Rectangle::transform() const {
   return this->drawable_.getTransform();
}

void Rectangle::size(float x, float y) {
   this->drawable_.setSize(sf::Vector2f(x, y));
}

void Rectangle::size(const sf::Vector2f& size) {
   this->drawable_.setSize(size);
}

const sf::Vector2f& Rectangle::size() const {
   return this->drawable_.getSize();
}

void Rectangle::outline_color(const sf::Color& color) {
   this->drawable_.setOutlineColor(color);
}

const sf::Color& Rectangle::outline_color() const {
   return this->drawable_.getOutlineColor();
}

void Rectangle::outline_thickness(float thickness) {
   this->drawable_.setOutlineThickness(thickness);
}

float Rectangle::outline_thickness() const {
   return this->drawable_.getOutlineThickness();
}

std::string Rectangle::serialize(Serializer& s) {
   Serializer::SerialData data;

   Color color(this->color());
   Color outline_color(this->outline_color());

   data["type"] = "Rectangle";

   data["id"] = this->id();
   data["x"] = std::to_string(this->position().x);
   data["y"] = std::to_string(this->position().y);
   data["width"] = std::to_string(this->size().x);
   data["height"] = std::to_string(this->size().y);
   data["rotation"] = std::to_string(this->rotation());
   data["origin_x"] = std::to_string(this->origin().x);
   data["origin_y"] = std::to_string(this->origin().y);
   data["color"] = color.serialize(s);
   data["outline_color"] = outline_color.serialize(s);
   data["outline_thickness"] = std::to_string(this->outline_thickness());

   return s.serialize(data);
}

void Rectangle::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   Color color(sf::Color::Black);
   Color outline_color(sf::Color::Black);

   color.deserialize(s, scene, data["color"]);
   outline_color.deserialize(s, scene, data["outline_color"]);

   this->id(data["id"]);
   this->position(std::stof(data["x"]), std::stof(data["y"]));
   this->size(std::stof(data["width"]), std::stof(data["height"]));
   this->rotation(std::stof(data["rotation"]));
   this->origin(std::stof(data["origin_x"]), std::stof(data["origin_y"]));
   this->color(color);
   this->outline_color(outline_color);
   this->outline_thickness(std::stof(data["outline_thickness"]));
}
