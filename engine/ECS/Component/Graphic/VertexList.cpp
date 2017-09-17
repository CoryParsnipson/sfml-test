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
, position_(other.position_)
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
   std::swap(this->position_, other.position_);
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
   
   bounds.left = this->position().x;
   bounds.top = this->position().y;

   return bounds;
}

void VertexList::position(float x, float y) {
   this->move(sf::Vector2f(x, y) - this->position_);

   this->position_.x = x;
   this->position_.y = y;
}

const sf::Vector2f& VertexList::position() const {
   return this->position_;
}

void VertexList::move(float x, float y) {
   this->render_states_.transform.translate(sf::Vector2f(x, y));

   this->position_.x += x;
   this->position_.y += y;
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
   this->position(x, y);
}

const sf::Vector2f& VertexList::origin() const {
   return this->position_;
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

   int color = this->color().r;
   color |= this->color().g << 8;
   color |= this->color().b << 16;
   color |= this->color().a << 24;

   data["type"] = "VertexList";

   data["id"] = this->id();
   data["x"] = std::to_string(this->position().x);
   data["y"] = std::to_string(this->position().y);
   data["rotation"] = std::to_string(this->rotation());
   data["origin_x"] = std::to_string(this->origin().x);
   data["origin_y"] = std::to_string(this->origin().y);
   data["color"] = std::to_string(color);
   data["primitive_type"] = std::to_string(static_cast<int>(this->drawable_.getPrimitiveType()));

   data["vertex_count"] = std::to_string(this->drawable_.getVertexCount());

   // vertex array info
   for (unsigned int i = 0; i < this->drawable_.getVertexCount(); ++i) {
      int v_color = this->vertex_color(i).r;
      v_color |= this->vertex_color(i).g << 8;
      v_color |= this->vertex_color(i).b << 16;
      v_color |= this->vertex_color(i).a << 24;

      data["vertex_" + std::to_string(i) + "_position_x"] = std::to_string(this->vertex_position(i).x);
      data["vertex_" + std::to_string(i) + "_position_y"] = std::to_string(this->vertex_position(i).y);
      data["vertex_" + std::to_string(i) + "_color"] = std::to_string(v_color);
      data["vertex_" + std::to_string(i) + "_texture_coord_x"] = std::to_string(this->vertex_texture_coord(i).x);
      data["vertex_" + std::to_string(i) + "_texture_coord_y"] = std::to_string(this->vertex_texture_coord(i).y);
   }
   
   return s.serialize(data);
}

void VertexList::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   int raw_color = std::stoi(data["color"]);

   int color_r = raw_color && 0xFF;
   int color_g = (raw_color && 0xFF00) >> 8;
   int color_b = (raw_color && 0xFF0000) >> 16;
   int color_a = (raw_color && 0xFF000000) >> 24;

   sf::Color color(color_r, color_g, color_b, color_a);

   this->id(data["id"]);
   this->position(std::stof(data["x"]), std::stof(data["y"]));
   this->rotation(std::stof(data["rotation"]));
   this->origin(std::stof(data["origin_x"]), std::stof(data["origin_y"]));
   this->color(color);
   this->drawable_.setPrimitiveType(static_cast<sf::PrimitiveType>(std::stoi(data["primitive_type"])));

   this->resize(std::stoi(data["vertex_count"]));

   // vertex array deserialization
   for (unsigned int i = 0; i < this->drawable_.getVertexCount(); ++i) {
      int raw_v_color = std::stoi(data["vertex_" + std::to_string(i) + "_color"]);

      int v_color_r = raw_v_color && 0xFF;
      int v_color_g = (raw_v_color && 0xFF00) >> 8;
      int v_color_b = (raw_v_color && 0xFF0000) >> 16;
      int v_color_a = (raw_v_color && 0xFF000000) >> 24;

      sf::Color v_color(v_color_r, v_color_g, v_color_b, v_color_a);

      this->vertex_position(i, sf::Vector2f(
         std::stof(data["vertex_" + std::to_string(i) + "_position_x"]),
         std::stof(data["vertex_" + std::to_string(i) + "_position_y"]))
      );

      this->vertex_color(i, v_color);

      this->vertex_texture_coord(i, sf::Vector2f(
         std::stof(data["vertex_" + std::to_string(i) + "_texture_coord_x"]),
         std::stof(data["vertex_" + std::to_string(i) + "_texture_coord_y"]))
      );
   }
}
