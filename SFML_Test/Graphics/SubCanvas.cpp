#include "SubCanvas.h"
#include "Camera.h"

SubCanvas::SubCanvas(const sf::Vector2f& size)
{
   this->surface_.create(Settings::Instance()->cur_width(), Settings::Instance()->cur_height());
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
