#include "Layer.h"

#include "Graphics.h"

// initialize static member variables
const float Layer::ZOOM_FACTOR_MIN = 0.125;
const float Layer::ZOOM_FACTOR_MAX = 3.0;

Layer::Layer(std::string id, sf::Vector2f size)
: id_(id)
, zoom_factor_(1.0)
, view_(nullptr)
{
   this->view_ = new sf::View(sf::Vector2f(size.x / 2.f, size.y / 2.f), size);
}

Layer::~Layer() {
   delete this->view_;

   // this do not own any drawables, so don't delete them
   this->drawables_.clear();
}

std::string Layer::id() {
   return this->id_;
}

std::string Layer::to_string() {
   return "[Layer: '" + this->id_ + "']";
}

sf::View& Layer::get_view() {
   return *this->view_;
}

void Layer::set_size(sf::Vector2f size) {
   this->view_->setSize(size * this->zoom_factor_);
}

void Layer::set_center(sf::Vector2f center) {
   this->view_->setCenter(center);
}

void Layer::set_viewport(const sf::FloatRect& viewport) {
   this->view_->setViewport(viewport);
}

sf::Vector2f Layer::get_size() {
   return this->view_->getSize() / this->zoom_factor_;
}

sf::Vector2f Layer::get_center() {
   return this->view_->getCenter();
}

sf::FloatRect Layer::get_viewport() {
   return this->view_->getViewport();
}

void Layer::add(Draw* drawable) {
   this->drawables_.push_back(drawable);
}

void Layer::remove(Draw* drawable) {
   DrawableList::const_iterator it;
   for (it = this->drawables_.begin(); it != this->drawables_.end(); ++it) {
      if (drawable == *it) {
         this->drawables_.erase(it);
         return;
      }
   }
}

void Layer::draw(Graphics& graphics) {
   DrawableList::const_iterator it;
   for (it = this->drawables_.begin(); it != this->drawables_.end();) {
      if (!(*it)) {
         Service::get_logger().msg("Layer", Logger::WARNING, "Removing null pointer from drawables list.");
         this->drawables_.erase(it);
         continue;
      }

      (*it)->draw(graphics, *this);
      ++it;
   }
}

void Layer::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   this->view_->move(delta);
}

float Layer::get_scale() {
   return this->zoom_factor_;
}

void Layer::set_scale(float factor) {
   factor = std::max(factor, Layer::ZOOM_FACTOR_MIN);
   factor = std::min(factor, Layer::ZOOM_FACTOR_MAX);
   Service::get_logger().msg("Layer", Logger::INFO, "Zoom factor: " + std::to_string(this->zoom_factor_));

   // update viewport size
   this->view_->zoom(factor / this->zoom_factor_);
   this->zoom_factor_ = factor;
}

void Layer::click(MouseButtonCommand& c) {}