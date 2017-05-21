#include "Rectangle.h"
#include "RenderSurface.h"

// ----------------------------------------------------------------------------
// initialize static members
// ----------------------------------------------------------------------------
template <>
ObjectPool<Rectangle> PooledComponent<Rectangle>::pool("Rectangle Component Pool", 50);

// ----------------------------------------------------------------------------
// Rectangle implementation
// ----------------------------------------------------------------------------
Rectangle::Rectangle()
: PooledComponent<Rectangle>("RectangleGraphic")
, drawable_(new sf::RectangleShape())
{
}

Rectangle::Rectangle(const std::string& id, float x /* = 0 */, float y /* = 0 */, float width /* = 100 */, float height /* = 100 */)
: PooledComponent<Rectangle>(id)
, drawable_(new sf::RectangleShape())
{
   this->drawable_->setSize(sf::Vector2f(width, height));
   this->drawable_->setPosition(x, y);
}

Rectangle::Rectangle(const std::string& id, sf::FloatRect rect /* = sf::FloatRect(0, 0, 100, 100) */)
: PooledComponent<Rectangle>(id)
, drawable_(new sf::RectangleShape())
{
   this->drawable_->setSize(sf::Vector2f(rect.width, rect.height));
   this->drawable_->setPosition(rect.left, rect.top);
}

Rectangle::~Rectangle() {
   delete this->drawable_;
}

void Rectangle::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(*this->drawable_, render_states);
}

sf::FloatRect Rectangle::local_bounds() const {
   return this->drawable_->getLocalBounds();
}

sf::FloatRect Rectangle::global_bounds() const {
   return this->drawable_->getGlobalBounds();
}

void Rectangle::position(float x, float y) {
   this->drawable_->setPosition(x, y);
}

const sf::Vector2f& Rectangle::position() const {
   return this->drawable_->getPosition();
}

void Rectangle::move(float x, float y) {
   this->drawable_->move(x, y);
}

void Rectangle::rotation(float angle) {
   this->drawable_->setRotation(angle);
}

float Rectangle::rotation() const {
   return this->drawable_->getRotation();
}

void Rectangle::scale(float x, float y) {
   this->drawable_->setScale(x, y);
}

const sf::Vector2f& Rectangle::scale() const {
   return this->drawable_->getScale();
}

void Rectangle::origin(float x, float y) {
   this->drawable_->setOrigin(x, y);
}

const sf::Vector2f& Rectangle::origin() const {
   return this->drawable_->getOrigin();
}

void Rectangle::color(const sf::Color& color) {
   this->drawable_->setFillColor(color);
}

const sf::Color& Rectangle::color() const {
   return this->drawable_->getFillColor();
}

const sf::Transform& Rectangle::transform() const {
   return this->drawable_->getTransform();
}

void Rectangle::size(float x, float y) {
   this->drawable_->setSize(sf::Vector2f(x, y));
}

void Rectangle::size(const sf::Vector2f& size) {
   this->drawable_->setSize(size);
}

const sf::Vector2f& Rectangle::size() const {
   return this->drawable_->getSize();
}

void Rectangle::outline_color(const sf::Color& color) {
   this->drawable_->setOutlineColor(color);
}

const sf::Color& Rectangle::outline_color() const {
   return this->drawable_->getOutlineColor();
}

void Rectangle::outline_thickness(float thickness) {
   this->drawable_->setOutlineThickness(thickness);
}

float Rectangle::outline_thickness() const {
   return this->drawable_->getOutlineThickness();
}
