#include "Graphics.h"

#include "GraphicsComponent.h"

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

void Graphics::update() {
   this->window->display();
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

void Graphics::clear() {
   this->window->clear();
}

sf::RenderWindow& Graphics::get_window() {
   return *(this->window);
}
