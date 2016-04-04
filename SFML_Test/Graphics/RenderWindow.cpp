#include "RenderWindow.h"
#include "Layer.h"

RenderWindow::RenderWindow(const std::string& title, const sf::Vector2f& size)
: surface_(sf::VideoMode(size.x, size.y), title)
{
   this->surface_.setFramerateLimit(Settings::Instance()->FRAMERATE_LIMIT);
   this->surface_.setMouseCursorVisible(false);
}

RenderWindow::~RenderWindow() {
}

bool RenderWindow::poll_event(sf::Event& event) {
   return this->surface_.pollEvent(event);
}

void RenderWindow::set_active_layer(Layer& layer) {
   this->surface_.setView(layer.get_view());
}

sf::Vector2f RenderWindow::get_world_coord(const sf::Vector2i& point, Layer* layer) {
   return (layer ? this->surface_.mapPixelToCoords(point, layer->get_view()) : this->surface_.mapPixelToCoords(point));
}

sf::Vector2i RenderWindow::get_screen_coord(const sf::Vector2f& point, Layer* layer) {
   return (layer ? this->surface_.mapCoordsToPixel(point, layer->get_view()) : this->surface_.mapCoordsToPixel(point));
}

void RenderWindow::draw(sf::Drawable& d) {
   this->surface_.draw(d);
}

void RenderWindow::draw(sf::Drawable& d, Layer& layer) {
   sf::View original_view = this->surface_.getView();

   this->set_active_layer(layer);
   this->draw(d);

   this->surface_.setView(original_view);
}

void RenderWindow::update() {
   this->surface_.display();
}

void RenderWindow::clear() {
   this->surface_.clear();
}

void RenderWindow::clear(const sf::Color& color) {
   this->surface_.clear(color);
}
