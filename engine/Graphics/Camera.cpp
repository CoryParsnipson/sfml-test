#include "Camera.h"
#include "Service.h"
#include "RenderSurface.h"
#include "TextFactory.h"
#include "FitCameraResizePolicy.h"

// initialize static member variables
const float Camera::ZOOM_FACTOR_MIN = 0.125;
const float Camera::ZOOM_FACTOR_MAX = 3.0;

Camera::Camera(const std::string& id)
: SceneObject(id, true)
, id_(id)
, zoom_factor_(1.0)
, original_center_(0, 0)
, view_(new sf::View(this->original_center_, sf::Vector2f(0, 0)))
, resize_policy_(new FitCameraResizePolicy())
{
}

Camera::Camera(const std::string& id, const sf::Vector2f& size)
: SceneObject(id, true)
, id_(id)
, zoom_factor_(1.0)
, original_center_(size.x / 2.f, size.y / 2.f)
, view_(new sf::View(this->original_center_, size))
, resize_policy_(new FitCameraResizePolicy())
{
}

Camera::~Camera() {
   delete this->view_;
}

const std::string& Camera::id() const {
   return this->id_;
}

std::string Camera::to_string() const {
   return "[Camera: " + this->id() + "]";
}

void Camera::reset_pan() {
   sf::Vector2f pan_delta = this->original_center_ - this->get_center();
   this->move(pan_delta);
}

void Camera::reset_zoom() {
   this->set_scale(1.0);
}

sf::Vector2f Camera::get_pan_delta() {
   return (this->original_center_ - this->get_center());
}

void Camera::policy(std::shared_ptr<CameraResizePolicy> policy) {
   this->resize_policy_ = policy;
}

const std::shared_ptr<CameraResizePolicy>& Camera::policy() {
   return this->resize_policy_;
}

void Camera::resize(sf::Vector2f size) {
   if (this->resize_policy_) {
      this->resize_policy_->resize(*this, size);
   }
}

sf::Vector2f Camera::get_size() {
   return this->view_->getSize() / this->zoom_factor_;
}

void Camera::set_size(const sf::Vector2f& size) {
   this->view_->setSize(size * this->zoom_factor_);
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
   this->transform_.scale(factor / this->zoom_factor_, factor / this->zoom_factor_);

   this->zoom_factor_ = factor;
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

void Camera::move(const sf::Vector2f& delta) {
   this->view_->move(delta * this->get_scale());

   // transform (for child scene object positioning) moves in opposite direction of camera
   this->transform_.translate(-delta * this->get_scale());
}

const sf::View& Camera::view() const {
   return *this->view_;
}

void Camera::set_viewport(const sf::FloatRect& viewport) {
   this->view_->setViewport(viewport);
}

const sf::FloatRect& Camera::get_viewport() {
   return this->view_->getViewport();
}

sf::Vector2f Camera::get_world_coordinate(const sf::Vector2f& point) {
   return this->transform_.transformPoint(point);
}

sf::Vector2f Camera::get_screen_coordinate(const sf::Vector2f& point) {
   sf::IntRect viewport = this->viewport();
   sf::Vector2f screen_coord(
      -1.f + 2.f * (point.x - viewport.left) / viewport.width,
       1.f - 2.f * (point.y - viewport.top) / viewport.height
   );
   
   return this->view_->getInverseTransform().transformPoint(screen_coord);
}

bool Camera::intersects(const sf::Vector2i& other) {
   return this->intersects(static_cast<sf::Vector2f>(other));
}

bool Camera::intersects(const sf::Vector2f& other) {
   sf::FloatRect cam_box = static_cast<sf::FloatRect>(this->viewport());
   return cam_box.contains(other);
}

bool Camera::intersects(const sf::FloatRect& other) {
   sf::FloatRect cam_box = static_cast<sf::FloatRect>(this->viewport());
   return cam_box.intersects(other);
}

// TODO: implement me
bool Camera::intersects(const SceneObject& other) { return false; }

void Camera::accept(SceneGraphVisitor& visitor) {
   visitor.visit(this);
}

void Camera::do_draw(RenderSurface& surface, sf::RenderStates render_states) {
   surface.view(*this->view_);
}

std::string Camera::serialize(Serializer& s) {
   // TODO implement me
   return "";
}

void Camera::deserialize(Serializer& s, Scene& scene, std::string& d) {
   // TODO implement me
}

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const Camera& camera) {
   stream << camera.to_string();
   return stream;
}

sf::IntRect Camera::viewport() {
   // copy this directly from SFML source...
   sf::Vector2f size = this->view_->getSize() / this->get_scale();

   sf::FloatRect raw_viewport = this->view_->getViewport();
   sf::IntRect viewport(
      static_cast<int>(0.5f + size.x * raw_viewport.left),
      static_cast<int>(0.5f + size.y * raw_viewport.top),
      static_cast<int>(0.5f + size.x * raw_viewport.width),
      static_cast<int>(0.5f + size.y * raw_viewport.height)
   );

   return viewport;
}
