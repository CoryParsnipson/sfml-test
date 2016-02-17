#include "Graphics.h"

#include "Layer.h"

Graphics::Graphics()
{
   this->window = new sf::RenderWindow(sf::VideoMode(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT), "SFML Test");

	this->window->setFramerateLimit(Settings::Instance()->FRAMERATE_LIMIT);
	this->window->setMouseCursorVisible(false);
}

Graphics::Graphics(sf::RenderWindow& window)
: window(&window)
{
}

bool Graphics::is_open() {
   return this->window->isOpen();
}

void Graphics::close() {
   this->window->close();
}

void Graphics::clear() {
   this->window->clear();
}

sf::RenderWindow& Graphics::get_window() {
   return *(this->window);
}

sf::Vector2f Graphics::get_world_coord(const sf::Vector2i& point, Layer* layer) {
   return (layer ? this->window->mapPixelToCoords(point, layer->get_view()) : this->window->mapPixelToCoords(point));
}

sf::Vector2i Graphics::get_screen_coord(const sf::Vector2f& point, Layer* layer) {
   return (layer ? this->window->mapCoordsToPixel(point, layer->get_view()) : this->window->mapCoordsToPixel(point));
}

void Graphics::set_active_layer(Layer& layer) {
   this->window->setView(layer.get_view());
}

void Graphics::draw(sf::Drawable& d) {
   this->window->draw(d);
}

void Graphics::draw(sf::Drawable& d, Layer& layer) {
   this->window->setView(layer.get_view());
   this->window->draw(d);
}

void Graphics::update(Game& game, Scene* scene, Entity* entity) {
   this->window->display();
}

