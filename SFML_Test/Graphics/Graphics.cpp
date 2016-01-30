#include "Graphics.h"

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

void Graphics::draw(sf::Drawable& d, sf::View* view) {
   if (!view) {
      this->window->draw(d); 
      return;
   }

   sf::View original_view = this->window->getView();
   this->window->setView(*view);
   this->window->draw(d);
   this->window->setView(original_view);
}

void Graphics::update(Game& game, Scene* scene, Entity* entity) {
   this->window->display();
}

sf::Vector2f Graphics::get_world_coord(const sf::Vector2i& point, sf::View* view) {
   return (view ? this->window->mapPixelToCoords(point, *view) : this->window->mapPixelToCoords(point));
}

sf::Vector2i Graphics::get_screen_coord(const sf::Vector2f& point, sf::View* view) {
   return (view ? this->window->mapCoordsToPixel(point, *view) : this->window->mapCoordsToPixel(point));
}

sf::RenderWindow& Graphics::get_window() {
   return *(this->window);
}

void Graphics::write(std::string msg, sf::Vector2f pos, const FontConfig* config, sf::View* view) {
   sf::View original_view = this->window->getView();

   if (view) {
      this->window->setView(*view);
   }

   this->writer.write(*this, msg, pos, config);

   if (view) {
      this->window->setView(original_view);
   }
}

void Graphics::load_font(std::string font_name, std::string filename) {
   this->writer.load_font(font_name, filename);
}
