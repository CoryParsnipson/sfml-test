#include <cassert>
#include <algorithm>

#include "VertexList.h"
#include "RenderSurface.h"

// ----------------------------------------------------------------------------
// VertexList implementation
// ----------------------------------------------------------------------------
VertexList::VertexList(const std::string& id /* = VertexListGraphic */, sf::PrimitiveType primitive /* = sf::TrianglesStrip */, unsigned int size /* = 0 */)
: Component(id)
, scale_(1.0f, 1.0f)
, drawable_(primitive, size)
{
}

VertexList::VertexList(const VertexList& other)
: Component(other.id())
, rotation_(other.rotation_)
, size_(other.size_)
, offset_(other.offset_)
, scale_(other.scale_)
, render_states_(other.render_states_)
, drawable_(other.drawable_)
{
}

VertexList::~VertexList() {
}

VertexList& VertexList::operator=(const VertexList& other) {
   VertexList tmp(other);
   this->swap(tmp);
   return *this;
}

void VertexList::swap(VertexList& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));

   std::swap(this->rotation_, other.rotation_);
   std::swap(this->size_, other.size_);
   std::swap(this->offset_, other.offset_);
   std::swap(this->scale_, other.scale_);
   std::swap(this->render_states_, other.render_states_);
   std::swap(this->drawable_, other.drawable_);
}

void VertexList::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   render_states.transform *= this->render_states_.transform;
   surface.draw(this->drawable_, render_states);
}

sf::FloatRect VertexList::local_bounds() const {
   return this->drawable_.getBounds();
}

sf::FloatRect VertexList::global_bounds() const {
   sf::FloatRect bounds = this->drawable_.getBounds();
   
   bounds.left = this->offset().x;
   bounds.top = this->offset().y;

   return bounds;
}

void VertexList::rotation(float angle) {
   this->render_states_.transform.rotate(angle);
   this->rotation_ = static_cast<int>(this->rotation_ + angle) % 360;
}

float VertexList::rotation() const {
   return this->rotation_;
}

void VertexList::scale(float x, float y) {
   this->render_states_.transform.scale(x, y);

   this->scale_.x *= x;
   this->scale_.y *= y;

   this->size_.x *= x;
   this->size_.y *= y;
}

const sf::Vector2f& VertexList::scale() const {
   return this->scale_;
}

void VertexList::origin(float x, float y) {
   this->offset(x, y);
}

const sf::Vector2f& VertexList::origin() const {
   return this->offset_;
}

void VertexList::offset(float x, float y) {
   this->move_offset(sf::Vector2f(x, y) - this->offset_);

   this->offset_.x = x;
   this->offset_.y = y;
}

const sf::Vector2f& VertexList::offset() const {
   return this->offset_;
}

void VertexList::move_offset(float x, float y) {
   this->render_states_.transform.translate(sf::Vector2f(x, y));

   this->offset_.x += x;
   this->offset_.y += y;
}

void VertexList::color(const sf::Color& color) {
   for (unsigned int i = 0; i < this->drawable_.getVertexCount(); ++i) {
      this->drawable_[i].color = color;
   }
}

const sf::Color& VertexList::color() const {
   if (this->drawable_.getVertexCount() >= 1) {
      return this->drawable_[0].color;
   }
   return sf::Color::Transparent;
}

const sf::Transform& VertexList::transform() const {
   return this->render_states_.transform;
}

void VertexList::resize(unsigned int idx) {
   this->drawable_.resize(idx);
}

void VertexList::vertex(unsigned int idx, sf::Vertex vert) {
   if (idx > this->drawable_.getVertexCount()) {
      this->drawable_.resize(idx);
   }
   this->drawable_[idx] = vert;
}

sf::Vertex& VertexList::vertex(unsigned int idx) {
   if (idx > this->drawable_.getVertexCount()) {
      this->drawable_.resize(idx);
   }
   return this->drawable_[idx];
}

const sf::Vertex& VertexList::vertex(unsigned int idx) const {
   assert(idx < this->drawable_.getVertexCount());
   return this->drawable_[idx];
}

void VertexList::vertex_color(unsigned int idx, const sf::Color& color) {
   this->vertex(idx).color = color;
}

const sf::Color& VertexList::vertex_color(unsigned int idx) const {
   return this->vertex(idx).color;
}

void VertexList::vertex_position(unsigned int idx, const sf::Vector2f& pos) {
   this->vertex(idx).position = pos;
}

const sf::Vector2f& VertexList::vertex_position(unsigned int idx) const {
   return this->vertex(idx).position;
}

void VertexList::vertex_texture_coord(unsigned int idx, const sf::Vector2f& texture_coord) {
   this->vertex(idx).texCoords = texture_coord;
}

const sf::Vector2f& VertexList::vertex_texture_coord(unsigned int idx) const {
   return this->vertex(idx).texCoords;
}

std::string VertexList::serialize(Serializer& s) {
   Serializer::SerialData data;

   Color color(this->color());

   data["type"] = "VertexList";

   data["id"] = this->id();
   data["x"] = std::to_string(this->offset().x);
   data["y"] = std::to_string(this->offset().y);
   data["rotation"] = std::to_string(this->rotation());
   data["origin_x"] = std::to_string(this->origin().x);
   data["origin_y"] = std::to_string(this->origin().y);
   data["color"] = color.serialize(s);
   data["primitive_type"] = std::to_string(static_cast<int>(this->drawable_.getPrimitiveType()));

   data["vertex_count"] = std::to_string(this->drawable_.getVertexCount());

   // vertex array info
   for (unsigned int i = 0; i < this->drawable_.getVertexCount(); ++i) {
      Color v_color(this->vertex_color(i));

      data["vertex_" + std::to_string(i) + "_offset_x"] = std::to_string(this->vertex_position(i).x);
      data["vertex_" + std::to_string(i) + "_offset_y"] = std::to_string(this->vertex_position(i).y);
      data["vertex_" + std::to_string(i) + "_color"] = v_color.serialize(s);
      data["vertex_" + std::to_string(i) + "_texture_coord_x"] = std::to_string(this->vertex_texture_coord(i).x);
      data["vertex_" + std::to_string(i) + "_texture_coord_y"] = std::to_string(this->vertex_texture_coord(i).y);
   }
   
   return s.serialize(data);
}

void VertexList::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   Color color(sf::Color::Black);
   color.deserialize(s, scene, data["color"]);

   this->id(data["id"]);
   this->offset(std::stof(data["x"]), std::stof(data["y"]));
   this->rotation(std::stof(data["rotation"]));
   this->origin(std::stof(data["origin_x"]), std::stof(data["origin_y"]));
   this->color(color);
   this->drawable_.setPrimitiveType(static_cast<sf::PrimitiveType>(std::stoi(data["primitive_type"])));

   this->resize(std::stoi(data["vertex_count"]));

   // vertex array deserialization
   for (unsigned int i = 0; i < this->drawable_.getVertexCount(); ++i) {
      Color v_color(sf::Color::Black);
      v_color.deserialize(s, scene, data["vertex_" + std::to_string(i) + "_color"]);

      this->vertex_position(i, sf::Vector2f(
         std::stof(data["vertex_" + std::to_string(i) + "_offset_x"]),
         std::stof(data["vertex_" + std::to_string(i) + "_offset_y"]))
      );

      this->vertex_color(i, v_color);

      this->vertex_texture_coord(i, sf::Vector2f(
         std::stof(data["vertex_" + std::to_string(i) + "_texture_coord_x"]),
         std::stof(data["vertex_" + std::to_string(i) + "_texture_coord_y"]))
      );
   }
}
