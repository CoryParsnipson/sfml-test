#include "Canvas.h"
#include "Camera.h"

Canvas::Canvas(const std::string& title, const sf::Vector2f& size, int framerate_limit /* = 60 */)
: surface_(sf::VideoMode(size.x, size.y), title)
{
   this->set_framerate_limit(framerate_limit);
   this->set_mouse_cursor_visible(false);
}

Canvas::~Canvas() {
   this->surface_.close();
}

void Canvas::set_camera(Camera& camera) {
   this->surface_.setView(*camera.view_);
}

void Canvas::draw(Draw& drawable, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   // delay drawing until update, add to layers here
   this->layers_[drawable.layer()].push_back(std::make_tuple(&drawable, render_states));
}

void Canvas::draw(sf::Drawable& drawable, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->surface_.draw(drawable, render_states);
}

void Canvas::update() {
   // iterate through layers and draw to surface
   RenderSurface::LayerList::const_iterator current_layer;
   for (current_layer = this->layers_.begin(); current_layer != this->layers_.end(); ++current_layer) {
      Service::get_logger().msg("Canvas", Logger::INFO, "Drawing on layer " + std::to_string(current_layer->first));

      decltype(current_layer->second)::const_iterator draw_item;

      for (draw_item = current_layer->second.begin(); draw_item != current_layer->second.end(); ++draw_item) {
         std::get<0>(*draw_item)->draw(*this, static_cast<sf::RenderStates>(std::get<1>(*draw_item)));
      }
   }

   // clear the layer lists
   this->layers_.clear();

   // flush to screen
   this->surface_.display();
}

void Canvas::clear(const sf::Color* color /* = nullptr */) {
   if (!color) {
      this->surface_.clear();
      return;
   }

   this->surface_.clear(*color);
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
