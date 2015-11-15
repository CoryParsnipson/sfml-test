#include "Viewport.h"

// initialize static member variables
const float Viewport::ZOOM_FACTOR_MIN = 0.125;
const float Viewport::ZOOM_FACTOR_MAX = 3.0;

Viewport::Viewport(Game& game, sf::Vector2f size)
: game(game)
{
   this->default_size = size;

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

void Viewport::set_size(sf::Vector2f size) {
   this->view->setSize(size);
}

sf::Vector2f Viewport::get_size() {
   return this->view->getSize();
}

sf::Vector2f Viewport::get_default_size() {
   return this->default_size;
}

void Viewport::set_center(sf::Vector2f center) {
   this->view->setCenter(center);
}

sf::Vector2f Viewport::get_center() {
   return this->view->getCenter();
}

sf::Vector2f Viewport::get_default_center() {
   return this->default_center;
}

float Viewport::get_zoom_factor() {
   return this->zoom_factor;
}

void Viewport::set_zoom_factor(float new_zoom_factor) {
   new_zoom_factor = std::max(new_zoom_factor, Viewport::ZOOM_FACTOR_MIN);
   new_zoom_factor = std::min(new_zoom_factor, Viewport::ZOOM_FACTOR_MAX);

   this->zoom_factor = new_zoom_factor;
}
