#include "SceneObject.h"
#include "RenderSurface.h"

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

void SceneObject::on_mouse_in() {}
void SceneObject::on_mouse_out() {}
void SceneObject::on_click() {}
void SceneObject::on_release() {}

void SceneObject::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   if (!this->visible()) {
      return;
   }

   this->apply_transform(render_states);
   this->draw_pre(surface, render_states);

   SceneObject::iterator it;
   for (it = this->begin(); it != this->end(); ++it) {
      (*it)->draw(surface, render_states);
   }

   this->draw_post(surface, render_states);
}

void SceneObject::update(Game& game, Scene* scene /* = nullptr */) {}

void SceneObject::apply_transform(sf::RenderStates& render_states) {
   render_states.transform *= this->transform_;
}

void SceneObject::draw_pre(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {}
void SceneObject::draw_post(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {}
