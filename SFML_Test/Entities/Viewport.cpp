#include "Viewport.h"

// initialize static member variables
const float Viewport::ZOOM_FACTOR_MIN = 0.125;
const float Viewport::ZOOM_FACTOR_MAX = 3.0;

Viewport::Viewport(Game& game, sf::RenderWindow& window)
: game(game)
, window(window)
{
   this->default_size = static_cast<sf::Vector2f>(window.getSize());

   this->default_center.x = this->default_size.x / 2.f;
   this->default_center.y = this->default_size.y / 2.f;

   this->view = new sf::View(this->default_center, this->default_size);
}

Viewport::~Viewport() {
   delete this->view;
}

sf::View& Viewport::get_view() {
   return *(this->view);
}

void Viewport::move(sf::Vector2f delta) {
   this->view->move(delta);
}

float Viewport::get_zoom_factor() {
   return this->zoom_factor;
}

void Viewport::set_zoom_factor(float new_zoom_factor) {
   new_zoom_factor = std::max(new_zoom_factor, Viewport::ZOOM_FACTOR_MIN);
   new_zoom_factor = std::min(new_zoom_factor, Viewport::ZOOM_FACTOR_MAX);

   this->zoom_factor = new_zoom_factor;
}

void Viewport::draw(sf::Sprite& sprite) {
   this->window.setView(*(this->view));
   this->window.draw(sprite);
}

