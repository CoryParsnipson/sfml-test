#include "SceneObject.h"
#include "RenderSurface.h"
#include "Camera.h"
#include "Command.h"

SceneObject::SceneObject(bool visible /* = true */)
: Composite<SceneObject, true>()
, visible_(visible)
, propagate_event_(true)
, clickable_(false)
, on_mouse_in_(nullptr)
, on_mouse_out_(nullptr)
, on_mouse_move_(nullptr)
, on_click_(nullptr)
, on_left_click_(nullptr)
, on_right_click_(nullptr)
, on_release_(nullptr)
, on_left_release_(nullptr)
, on_right_release_(nullptr)
, on_mouse_wheel_(nullptr)
, transform_(sf::Transform::Identity)
{
}

SceneObject::~SceneObject() {
}

SceneObject::operator std::string() const {
   return this->to_string();
}

std::string SceneObject::to_string() const {
   return "[SceneObject]";
}

SceneObject* SceneObject::layer(int idx) const {
   return this->child(idx);
}

int SceneObject::num_layers() const {
   return this->num_children();
}

bool SceneObject::visible() const {
   return this->visible_;
}

void SceneObject::visible(bool visible) {
   this->visible_ = visible;
}

sf::Transform SceneObject::global_transform() const {
   if (!this->parent()) {
      return this->transform_;
   }
   return this->parent()->global_transform() * this->transform_;
}

sf::Transform SceneObject::local_transform() const {
   return this->transform_;
}

sf::Vector2f SceneObject::get_world_coordinate(const sf::Vector2f& point) {
   sf::Vector2f transformed_point = point;

   if (this->parent()) {
      transformed_point = this->parent()->get_world_coordinate(point);
   }
   return transformed_point;
}

sf::Vector2f SceneObject::get_screen_coordinate(const sf::Vector2f& point) {
   sf::Vector2f transformed_point = point;

   if (this->parent()) {
      transformed_point = this->parent()->get_screen_coordinate(point);
   }
   return transformed_point;
}

void SceneObject::propagate_event(bool propagate_event) {
   this->propagate_event_ = propagate_event;
}

bool SceneObject::propagate_event() const {
   return this->propagate_event_;
}

bool SceneObject::clickable() const {
   return this->clickable_;
}

void SceneObject::clickable(bool clickable) {
   this->clickable_ = clickable;
}

void SceneObject::on_mouse_in() {
   if (this->on_mouse_in_) {
      this->on_mouse_in_->execute();
   }
}

void SceneObject::on_mouse_in(Command* cmd) {
   this->on_mouse_in_ = cmd;
}

void SceneObject::on_mouse_out() {
   if (this->on_mouse_out_) {
      this->on_mouse_out_->execute();
   }
}

void SceneObject::on_mouse_out(Command* cmd) {
   this->on_mouse_out_ = cmd;
}

void SceneObject::on_mouse_move() {
   if (this->on_mouse_move_) {
      this->on_mouse_move_->execute();
   }
}

void SceneObject::on_mouse_move(Command* cmd) {
   this->on_mouse_move_ = cmd;
}

void SceneObject::on_click() {
   if (this->clickable() && this->on_click_) {
      this->on_click_->execute();
   }

   if (this->propagate_event() && this->parent()) {
      this->parent()->on_click();
   }
}

void SceneObject::on_click(Command* cmd) {
   this->on_click_ = cmd;
}

void SceneObject::on_left_click() {
   if (this->clickable() && this->on_left_click_) {
      this->on_left_click_->execute();
   }

   if (this->clickable() && this->on_click_) {
      this->on_click_->execute();
   }

   if (this->propagate_event() && this->parent()) {
      this->parent()->on_left_click();
   }
}

void SceneObject::on_left_click(Command* cmd) {
   this->on_left_click_ = cmd;
}

void SceneObject::on_right_click() {
   if (this->clickable() && this->on_right_click_) {
      this->on_right_click_->execute();
   }

   if (this->clickable() && this->on_click_) {
      this->on_click_->execute();
   }

   if (this->propagate_event() && this->parent()) {
      this->parent()->on_right_click();
   }
}

void SceneObject::on_right_click(Command* cmd) {
   this->on_right_click_ = cmd;
}

void SceneObject::on_release() {
   if (this->on_release_) {
      this->on_release_->execute();
   }

   if (this->propagate_event() && this->parent()) {
      this->parent()->on_release();
   }
}

void SceneObject::on_release(Command* cmd) {
   this->on_release_ = cmd;
}

void SceneObject::on_left_release() {
   if (this->on_left_release_) {
      this->on_left_release_->execute();
   }

   if (this->on_release_) {
      this->on_release_->execute();
   }

   if (this->propagate_event() && this->parent()) {
      this->parent()->on_left_release();
   }
}

void SceneObject::on_left_release(Command* cmd) {
   this->on_left_release_ = cmd;
}

void SceneObject::on_right_release() {
   if (this->on_right_release_) {
      this->on_right_release_->execute();
   }

   if (this->on_release_) {
      this->on_release_->execute();
   }

   if (this->propagate_event() && this->parent()) {
      this->parent()->on_right_release();
   }
}

void SceneObject::on_right_release(Command* cmd) {
   this->on_right_release_ = cmd;
}

void SceneObject::on_mouse_wheel() {
   if (this->on_mouse_wheel_) {
      this->on_mouse_wheel_->execute();
   }
}

void SceneObject::on_mouse_wheel(Command* cmd) {
   this->on_mouse_wheel_ = cmd;
}

void SceneObject::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   if (!this->visible()) {
      return;
   }

   this->do_draw(surface, render_states);
}

void SceneObject::update(Game& game, Scene* scene /* = nullptr */) {}

void SceneObject::do_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {}

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const SceneObject& object) {
   stream << object.to_string();
   return stream;
}