#include "Game.h"
#include "Camera.h"
#include "FitCameraResizePolicy.h"
#include "RenderSurface.h"

Camera::Camera(const std::string& id)
: id_(id)
, view_()
, transform_()
, resize_policy_(new FitCameraResizePolicy())
{
}

Camera::Camera(const std::string& id, const sf::Vector2f& size)
: id_(id)
, view_(size / 2.f, size)
, transform_()
, resize_policy_(new FitCameraResizePolicy())
{
}

Camera::Camera(const Camera& other)
: id_(other.id())
, view_(other.view_)
, transform_(other.transform_)
, resize_policy_(other.resize_policy_)
{
}

Camera::~Camera() {
}

Camera& Camera::operator=(const Camera& other) {
   Camera tmp(other);
   this->swap(tmp);
   return *this;
}

void Camera::swap(Camera& other) {
   std::swap(this->id_, other.id_);
   std::swap(this->view_, other.view_);
   std::swap(this->transform_, other.transform_);
   std::swap(this->resize_policy_, other.resize_policy_);
}

const std::string& Camera::id() const {
   return this->id_;
}

void Camera::id(std::string id) {
   this->id_ = id;
}

std::string Camera::to_string() const {
   return "[Camera: " + this->id() + "]";
}

void Camera::draw(RenderSurface& surface) {
   surface.draw(this);
}

void Camera::policy(CameraResizePolicyPtr policy) {
   this->resize_policy_ = policy;
}

CameraResizePolicyPtr Camera::policy() {
   return this->resize_policy_;
}

const sf::Vector2f& Camera::center() const {
   return this->view_.getCenter();
}

void Camera::center(const sf::Vector2f& center) {
   this->center(center.x, center.y);
}

void Camera::center(float x, float y) {
   this->transform_.move(sf::Vector2f(x, y) - this->center());
   this->view_.setCenter(x, y); // center (change position) this camera at x, y
}

const sf::Vector2f& Camera::size() const {
   return this->view_.getSize();
}

void Camera::size(const sf::Vector2f& size) {
   assert(this->resize_policy_);
   this->resize_policy_->resize(*this, size);
}

void Camera::size(float x, float y) {
   this->size(sf::Vector2f(x, y));
}

float Camera::rotation() const {
   return this->view_.getRotation();
}

void Camera::rotation(float angle) {
   this->view_.setRotation(angle);
}

float Camera::zoom() const {
   return this->transform_.getScale().x; // can only scale camera proportionally
}

sf::Vector2f Camera::position() const {
   return this->center() - (this->size() / 2.f);
}

void Camera::position(const sf::Vector2f& position) {
   this->move(position - this->position());
}

void Camera::position(float x, float y) {
   this->position(sf::Vector2f(x, y));
}

void Camera::zoom(float factor) {
   this->view_.zoom(factor / this->zoom());
   this->transform_.setScale(factor, factor);
}

void Camera::move(const sf::Vector2f& delta) {
   this->view_.move(delta);
   this->transform_.move(delta);
}

void Camera::move(float x, float y) {
   this->view_.move(x, y);
   this->transform_.move(x, y);
}

sf::FloatRect Camera::bounds() const {
   return sf::FloatRect(this->center() - this->size() / 2.f, this->size());
}

const sf::Transform& Camera::transform() const {
   return this->view_.getTransform();
}

const sf::Transform& Camera::inverse_transform() const {
   return this->view_.getInverseTransform();
}

void Camera::viewport(const sf::FloatRect& viewport) {
   this->view_.setViewport(viewport);
}

const sf::FloatRect& Camera::viewport() {
   return this->view_.getViewport();
}

sf::Vector2f Camera::get_world_coordinate(const sf::Vector2f& point) {
   // copied from SFML source (RenderTarget.cpp:mapPixelToCoords)
   sf::IntRect viewport = this->absolute_viewport();
   sf::Vector2f screen_coord(
      -1.f + 2.f * (point.x - viewport.left) / viewport.width,
       1.f - 2.f * (point.y - viewport.top) / viewport.height
   );
   
   return this->view_.getInverseTransform().transformPoint(screen_coord);
}

sf::Vector2f Camera::get_screen_coordinate(const sf::Vector2f& point) {
   // copied from SFML source (RenderTarget.cpp:mapCoordsToPixel)
   sf::Vector2f pixel;
   sf::Vector2f normalized = this->view_.getTransform().transformPoint(point);
   sf::IntRect viewport = this->absolute_viewport();

   pixel.x = (normalized.x + 1.f) / 2.f * viewport.width + viewport.left;
   pixel.y = (-normalized.y + 1.f) / 2.f * viewport.height + viewport.top;

   return pixel;
}

sf::IntRect Camera::absolute_viewport() {
   // copy this directly from SFML source...
   sf::Vector2f size = this->size() / this->zoom();

   sf::FloatRect raw_viewport = this->viewport();
   sf::IntRect viewport(
      static_cast<int>(0.5f + size.x * raw_viewport.left),
      static_cast<int>(0.5f + size.y * raw_viewport.top),
      static_cast<int>(0.5f + size.x * raw_viewport.width),
      static_cast<int>(0.5f + size.y * raw_viewport.height)
   );

   return viewport;
}

void Camera::resize(const sf::Vector2f& size) {
   this->view_.setSize(size);
}

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const Camera& camera) {
   stream << camera.to_string();
   return stream;
}
