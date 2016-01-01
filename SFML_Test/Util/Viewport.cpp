#include "Viewport.h"
#include "Graphics.h"

// initialize static member variables
const float Viewport::ZOOM_FACTOR_MIN = 0.125;
const float Viewport::ZOOM_FACTOR_MAX = 3.0;

Viewport::Viewport(Graphics& graphics, sf::Vector2f size)
: graphics(graphics)
, zoom_factor(1.0)
{
   this->default_size = size;

   this->default_center.x = this->default_size.x / 2.f;
   this->default_center.y = this->default_size.y / 2.f;

   Service::get_logger().msg(
      "Viewport",
      Logger::INFO,
      "default center (" + std::to_string(this->default_center.x) + ", " + std::to_string(this->default_center.y) + ")"
   );

   this->view = new sf::View(this->default_center, this->default_size);
}

Viewport::~Viewport() {
   delete this->view;
}

sf::View& Viewport::get_view() {
   return *(this->view);
}

Graphics& Viewport::get_graphics() {
   return this->graphics;
}

void Viewport::draw(sf::Drawable& d) {
   this->graphics.draw(d, this->view);
}

void Viewport::write(std::string msg, sf::Vector2f pos, const FontConfig* config) {
   this->graphics.write(msg, pos, config, &this->get_view());
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

void Viewport::set_default_size(sf::Vector2f size) {
   this->default_size = size;
   
   // don't forget to update viewport size
   this->set_zoom_factor(this->zoom_factor);
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

void Viewport::set_default_center(sf::Vector2f size) {
   this->default_center = size;
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

   Service::get_logger().msg("Viewport", Logger::INFO, "Zoom factor: " + std::to_string(this->zoom_factor));

   // update viewport size
   this->set_size(this->zoom_factor * this->get_default_size());
}

sf::Vector2f Viewport::mapPixelToCoords(const sf::Vector2i& point) {
   return this->graphics.get_window().mapPixelToCoords(point, *this->view);
}

sf::Vector2i Viewport::mapCoordsToPixel(const sf::Vector2f& point) {
   return this->graphics.get_window().mapCoordsToPixel(point, *this->view);
}
