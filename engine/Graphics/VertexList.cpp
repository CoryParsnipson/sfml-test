#include <cassert>

#include "VertexList.h"
#include "RenderSurface.h"

VertexList::VertexList(const std::string& id, sf::PrimitiveType primitive /* = sf::TrianglesStrip */, unsigned int size /* = 0 */)
: Graphic2(id)
, scale_(1.0f, 1.0f)
, drawable_(new sf::VertexArray(primitive, size))
{
}

VertexList::~VertexList() {
   delete this->drawable_;
}

void VertexList::init() {
   // TODO implement for pooling
}

void VertexList::reset() {
   // TODO implement for pooling
}

void VertexList::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   render_states.transform *= this->render_states_.transform;
   surface.draw(*this->drawable_, render_states);
}

sf::FloatRect VertexList::local_bounds() const {
   return this->drawable_->getBounds();
}

sf::FloatRect VertexList::global_bounds() const {
   sf::FloatRect bounds = this->drawable_->getBounds();
   
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
   for (unsigned int i = 0; i < this->drawable_->getVertexCount(); ++i) {
      (*this->drawable_)[i].color = color;
   }
}

const sf::Color& VertexList::color() const {
   if (this->drawable_->getVertexCount() >= 1) {
      return (*this->drawable_)[0].color;
   }
   return sf::Color::Transparent;
}

const sf::Transform& VertexList::transform() const {
   return this->render_states_.transform;
}

void VertexList::accept(GraphicVisitor& visitor) {
   visitor.visit(this);
}

void VertexList::resize(unsigned int idx) {
   this->drawable_->resize(idx);
}

void VertexList::vertex(unsigned int idx, sf::Vertex vert) {
   if (idx > this->drawable_->getVertexCount()) {
      this->drawable_->resize(idx);
   }
   (*this->drawable_)[idx] = vert;
}

sf::Vertex& VertexList::vertex(unsigned int idx) {
   if (idx > this->drawable_->getVertexCount()) {
      this->drawable_->resize(idx);
   }
   return (*this->drawable_)[idx];
}

const sf::Vertex& VertexList::vertex(unsigned int idx) const {
   assert(idx < this->drawable_->getVertexCount());
   return (*this->drawable_)[idx];
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
