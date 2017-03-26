#include "Circle.h"
#include "RenderSurface.h"

Circle::Circle()
: Graphic2("CircleGraphic")
, drawable_(new sf::CircleShape())
{
}

Circle::Circle(const std::string& id, float radius /* = 10 */, float x /* = 0 */, float y /* = 0 */)
: Graphic2(id)
, drawable_(new sf::CircleShape(radius))
{
   this->drawable_->setPosition(x, y);
}

Circle::Circle(const std::string& id, float radius /* = 10 */, sf::Vector2f pos /* = sf::Vector2f(0, 0) */)
: Graphic2(id)
, drawable_(new sf::CircleShape(radius))
{
   this->drawable_->setPosition(pos);
}

Circle::~Circle() {
   delete this->drawable_;
}

void Circle::init() {
   // TODO: implement for pooling
}

void Circle::reset() {
   // TODO: implement for pooling
}

void Circle::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(*this->drawable_, render_states);
}

sf::FloatRect Circle::local_bounds() const {
   return this->drawable_->getLocalBounds();
}

sf::FloatRect Circle::global_bounds() const {
   return this->drawable_->getGlobalBounds();
}

void Circle::position(float x, float y) {
   this->drawable_->setPosition(x, y);
}

const sf::Vector2f& Circle::position() const {
   return this->drawable_->getPosition();
}

void Circle::move(float x, float y) {
   this->drawable_->move(x, y);
}

void Circle::rotation(float angle) {
   this->drawable_->setRotation(angle);
}

float Circle::rotation() const {
   return this->drawable_->getRotation();
}

void Circle::scale(float x, float y) {
   this->drawable_->setScale(x, y);
}

const sf::Vector2f& Circle::scale() const {
   return this->drawable_->getScale();
}

void Circle::origin(float x, float y) {
   this->drawable_->setOrigin(x, y);
}

const sf::Vector2f& Circle::origin() const {
   return this->drawable_->getOrigin();
}

void Circle::color(const sf::Color& color) {
   this->drawable_->setFillColor(color);
}

const sf::Color& Circle::color() const {
   return this->drawable_->getFillColor();
}

const sf::Transform& Circle::transform() const {
   return this->drawable_->getTransform();
}

void Circle::accept(GraphicVisitor& visitor) {
   visitor.visit(this);
}

void Circle::radius(float radius) {
   this->drawable_->setRadius(radius);
}

float Circle::radius() const {
   return this->drawable_->getRadius();
}

void Circle::outline_color(const sf::Color& color) {
   this->drawable_->setOutlineColor(color);
}

const sf::Color& Circle::outline_color() const {
   return this->drawable_->getOutlineColor();
}

void Circle::outline_thickness(float thickness) {
   this->drawable_->setOutlineThickness(thickness);
}

float Circle::outline_thickness() const {
   return this->drawable_->getOutlineThickness();
}