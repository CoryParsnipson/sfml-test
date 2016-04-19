#include "Camera.h"
#include "RenderSurface.h"

// initialize static member variables
const float Camera::ZOOM_FACTOR_MIN = 0.125;
const float Camera::ZOOM_FACTOR_MAX = 3.0;

Camera::Camera(const std::string& id, const sf::Vector2f& size)
: id_(id)
, zoom_factor_(1.0)
, original_center_(size.x / 2.f, size.y / 2.f)
, state_(sf::Transform::Identity)
, view_(nullptr)
{
   this->view_ = new sf::View(this->original_center_, size);
}

Camera::~Camera() {
   delete this->view_;
}

const std::string& Camera::id() {
   return this->id_;
}

const std::string& Camera::to_string() {
   return this->id();
}

void Camera::reset_pan() {
   sf::Vector2f pan_delta = this->original_center_ - this->get_center();
   this->view_->move(pan_delta);
   this->state_.translate(pan_delta);
}

void Camera::reset_zoom() {
   this->set_scale(1.0);
   this->state_.scale(1 / this->zoom_factor_, 1 / this->zoom_factor_);
}

sf::Vector2f Camera::get_pan_delta() {
   return (this->original_center_ - this->get_center());
}

void Camera::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.set_camera(*this);
}

sf::Vector2f Camera::get_size() {
   return this->view_->getSize() / this->zoom_factor_;
}

void Camera::set_size(const sf::Vector2f& size) {
   this->view_->setSize(size * this->zoom_factor_);
}

const sf::Vector2f& Camera::get_center() {
   return this->view_->getCenter();
}

void Camera::set_center(const sf::Vector2f& center) {
   sf::Vector2f pan_delta = this->original_center_ - this->get_center();

   // recenter view
   this->view_->setCenter(center);

   // re-apply panning
   this->view_->move(-pan_delta);

   // update original center
   this->original_center_ = center;
}

const sf::Transform& Camera::get_transform() const {
   return this->state_;
}

void Camera::set_viewport(const sf::FloatRect& viewport) {
   this->view_->setViewport(viewport);
}

const sf::FloatRect& Camera::get_viewport() {
   return this->view_->getViewport();
}

void Camera::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   this->view_->move(-delta); // invert delta to make it act like a pan
   this->state_.translate(-delta);
}

float Camera::get_scale() {
   return this->zoom_factor_;
}

void Camera::set_scale(float factor) {
   factor = std::max(factor, Camera::ZOOM_FACTOR_MIN);
   factor = std::min(factor, Camera::ZOOM_FACTOR_MAX);
   Service::get_logger().msg(this->id(), Logger::INFO, "Zoom factor: " + std::to_string(factor));

   // update viewport size
   this->view_->zoom(factor / this->zoom_factor_);
   this->state_.scale(factor / this->zoom_factor_, factor / this->zoom_factor_);

   this->zoom_factor_ = factor;
}

void Camera::click(MouseButtonCommand& c) {}
