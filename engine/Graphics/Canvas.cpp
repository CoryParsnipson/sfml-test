#include "Canvas.h"

Canvas::Canvas(const std::string& title, const sf::Vector2f& size, int framerate_limit /* = 60 */)
: surface_(sf::VideoMode(size.x, size.y), title)
{
   this->set_framerate_limit(framerate_limit);
   this->set_mouse_cursor_visible(false);
}

Canvas::~Canvas() {
   this->surface_.close();
}

sf::Vector2f Canvas::size() const {
   return static_cast<sf::Vector2f>(this->surface_.getSize());
}

void Canvas::draw(sf::Drawable& drawable, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->surface_.draw(drawable, render_states);
}

void Canvas::update() {
   // flush to screen
   this->surface_.display();
}

void Canvas::clear(const sf::Color& color /* = sf::Color::Black */) {
   this->surface_.clear(color);
}

void Canvas::set_framerate_limit(int limit) {
   this->surface_.setFramerateLimit(limit);
}

void Canvas::set_mouse_cursor_visible(bool visible) {
   this->surface_.setMouseCursorVisible(visible);
}

bool Canvas::poll_event(sf::Event& event) {
   return this->surface_.pollEvent(event);
}

const sf::View& Canvas::view() const {
   return this->surface_.getView();
}

void Canvas::view(const sf::View& view) {
   this->surface_.setView(view);
}
