#include "SubCanvas.h"
#include "Camera.h"

SubCanvas::SubCanvas(sf::FloatRect bounds)
{
   this->bounds(bounds);
}

SubCanvas::SubCanvas(const sf::Vector2f& size)
{
   this->bounds(0, 0, size.x, size.y);
}

SubCanvas::~SubCanvas() {
}

sf::Vector2f SubCanvas::size() const {
   return static_cast<sf::Vector2f>(this->surface_.getSize());
}

void SubCanvas::draw(sf::Drawable& drawable, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   render_states.transform.translate(-this->bounds_.left, -this->bounds_.top);
   this->surface_.draw(drawable, render_states);
}

void SubCanvas::update() {
   // flush all drawables to sub canvas
   this->surface_.display();
}

void SubCanvas::clear(const sf::Color& color /* = sf::Color::Black */) {
   //this->surface_.clear(color);
   this->surface_.clear(sf::Color::Red);
}

const sf::Texture& SubCanvas::get_texture() {
   return this->surface_.getTexture();
}

void SubCanvas::offset(sf::Vector2f offset) {
   this->offset(offset.x, offset.y);
}

void SubCanvas::offset(float offset_x, float offset_y) {
   this->bounds_.left = offset_x;
   this->bounds_.top = offset_y;
}

const sf::FloatRect& SubCanvas::bounds() {
   return this->bounds_;
}

void SubCanvas::bounds(sf::FloatRect bounds) {
   if (this->bounds_.width < bounds.width || this->bounds_.height < bounds.height) {
      this->surface_.create(bounds.width, bounds.height);
      this->clear();
   }
   this->bounds_ = bounds;
}

void SubCanvas::bounds(sf::Vector2f pos, sf::Vector2f size) {
   this->bounds(sf::FloatRect(pos.x, pos.y, size.x, size.y));
}

void SubCanvas::bounds(float left, float top, float width, float height) {
   this->bounds(sf::FloatRect(left, top, width, height));
}

const sf::View& SubCanvas::view() const {
   return this->surface_.getView();
}

void SubCanvas::view(const sf::View& view) {
   this->surface_.setView(view);
}
