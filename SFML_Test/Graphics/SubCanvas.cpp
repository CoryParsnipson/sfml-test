#include "SubCanvas.h"
#include "Camera.h"

SubCanvas::SubCanvas(const sf::Vector2f& size)
{
   this->set_size(size);
}

SubCanvas::~SubCanvas() {
}

void SubCanvas::set_camera(Camera& camera) {
   this->surface_.setView(*camera.view_);
}

void SubCanvas::draw(sf::Drawable& drawable, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->surface_.draw(drawable, render_states);
}

void SubCanvas::update() {
   // flush all drawables to sub canvas
   this->surface_.display();
}

void SubCanvas::clear(const sf::Color& color /* = sf::Color::Black */) {
   this->surface_.clear(color);
}

const sf::Texture& SubCanvas::get_texture() {
   return this->surface_.getTexture();
}

void SubCanvas::set_size(const sf::Vector2f& size) {
   this->set_size(size.x, size.y);
}

void SubCanvas::set_size(int size_x, int size_y) {
   this->surface_.create(size_x, size_y);
   this->surface_.clear();
}
