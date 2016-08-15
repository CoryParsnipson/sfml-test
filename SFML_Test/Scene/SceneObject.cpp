#include "SceneObject.h"
#include "RenderSurface.h"
#include "Camera.h"

SceneObject::SceneObject(bool visible /* = true */)
: Composite<SceneObject, true>()
, visible_(visible)
, transform_(sf::Transform::Identity)
{
}

SceneObject::~SceneObject() {
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

sf::Transform SceneObject::transform() const {
   if (!this->parent()) {
      return this->transform_;
   }
   return this->parent()->transform() * this->transform_;
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

void SceneObject::on_mouse_in() {}
void SceneObject::on_mouse_out() {}
void SceneObject::on_click() {}
void SceneObject::on_release() {}

void SceneObject::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   if (!this->visible()) {
      return;
   }

   this->apply_transform(surface, render_states);
   this->do_draw(surface, render_states);
}

void SceneObject::update(Game& game, Scene* scene /* = nullptr */) {}

void SceneObject::apply_transform(RenderSurface& surface, sf::RenderStates& render_states) {
   if (this->parent()) {
      this->parent()->apply_transform(surface, render_states);
   }
   this->do_apply_transform(surface, render_states);
}

void SceneObject::do_apply_transform(RenderSurface& surface, sf::RenderStates& render_states) {
   render_states.transform *= this->transform_;
}

void SceneObject::do_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {}
