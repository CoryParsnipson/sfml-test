#include <stack>
#include <tuple>
#include "assert.h"

#include "SceneRenderer.h"
#include "SceneObject.h"
#include "RenderSurface.h"
#include "Camera.h"
#include "Entity.h"
#include "Map.h"
#include "Grid.h"
#include "Widget.h"
#include "Gamepad.h"

void SceneRenderer::visit(Camera* camera) {
   if (std::get<1>(this->nodes_.top())) {
      // we have seen this camera, pop it and apply the previous camera
      assert(this->cameras_.top() == camera);
      this->cameras_.pop();
      this->pop();

      if (!this->cameras_.empty()) {
         this->cameras_.top()->draw(*this->surface_);
      }
   } else {
      // if this is a new camera, do draw routine + camera stack push
      this->cameras_.push(camera);
      this->draw();
   }
}

void SceneRenderer::visit(Entity* entity) {
   this->draw(entity->local_transform());
}

void SceneRenderer::visit(Map* map) {
   this->draw(map->local_transform());
}

void SceneRenderer::visit(Grid* grid) {
   this->draw(grid->local_transform());
}

void SceneRenderer::visit(Widget* widget) {
   this->draw(widget->local_transform());
}

void SceneRenderer::visit(Gamepad* gamepad) {
   this->draw(gamepad->local_transform());
}

void SceneRenderer::render(SceneObject& scene_graph, RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->surface_ = &surface;
   this->push(&scene_graph, render_states); // push root to node list

   // this does a pre-order traversal of the provided scene_graph scene object
   while (!this->nodes_.empty()) {
      std::get<0>(this->nodes_.top())->accept(*this); // do specialized processing
   }

   this->surface_ = nullptr;
}

void SceneRenderer::push(SceneObject* node, sf::RenderStates render_states) {
   this->nodes_.push(std::make_tuple(node, false));
   this->render_states_.push(render_states);
}

void SceneRenderer::pop() {
   this->nodes_.pop();
   this->render_states_.pop();
}

void SceneRenderer::draw(sf::Transform local_transform /* = sf::Transform::Identity */) {
   if (std::get<1>(this->nodes_.top())) {
      // if we've drawn this node before, pop it
      this->pop();
   } else {
      SceneObject* node = std::get<0>(this->nodes_.top());

      // draw the top node
      std::get<1>(this->nodes_.top()) = true;
      node->draw(*this->surface_, this->render_states_.top());

      // shortcut
      if (node->num_children() == 0) {
         this->pop();
         return;
      }

      sf::RenderStates new_render_states = this->render_states_.top();
      new_render_states.transform *= local_transform;

      // push node's immediate children (push to stack descending order to draw child 0 first)
      for (int i = node->num_children() - 1; i >= 0; --i) {
         this->push(node->child(i), new_render_states);
      }
   }
}
