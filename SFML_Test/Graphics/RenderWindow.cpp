#include "RenderWindow.h"
#include "Camera.h"

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

void RenderWindow::set_camera(Camera& camera) {
   this->surface_.setView(camera.get_view());
}

sf::Vector2f RenderWindow::get_world_coord(const sf::Vector2i& point, Camera* camera) {
   return (camera ? this->surface_.mapPixelToCoords(point, camera->get_view()) : this->surface_.mapPixelToCoords(point));
}

sf::Vector2i RenderWindow::get_screen_coord(const sf::Vector2f& point, Camera* camera) {
   return (camera ? this->surface_.mapCoordsToPixel(point, camera->get_view()) : this->surface_.mapCoordsToPixel(point));
}

void RenderWindow::draw(sf::Drawable& d, int layer) {
   // TODO: handle layers
   this->surface_.draw(d);
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
