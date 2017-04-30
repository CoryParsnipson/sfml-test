#include "GraphicsSystem.h"
#include "Scene.h"
#include "SceneObject.h"
#include "SystemVisitor.h"

GraphicsSystem::GraphicsSystem(const std::string& id /* = "GraphicsSystem" */)
: System(id)
{
}

GraphicsSystem::~GraphicsSystem() {
   for (CameraList::const_iterator it = this->cameras_.begin(); it != this->cameras_.end(); ++it) {
      delete *it;
   }
   this->cameras_.clear();
}

void GraphicsSystem::update(Game& game) {
   Scene* scene = game.current_scene();
   if (!scene) {
      return;
   }

   this->window_ = &game.window();
   
   for (SceneObject::prefix_iterator it = scene->scene_graph()->begin(); it != scene->scene_graph()->end(); ++it) {
      (*it)->accept(*this);
   }
}

void GraphicsSystem::accept(SystemVisitor& visitor) {
   visitor.visit(this);
}

void GraphicsSystem::visit(Camera*) {}

void GraphicsSystem::visit(Entity* e) {
   //Graphic2* g = e->get<Graphic2>();

   //if (g) {
   //   g->draw(*this->window_);
   //}
}

void GraphicsSystem::visit(Map*) {}
void GraphicsSystem::visit(Grid*) {}
void GraphicsSystem::visit(Widget*) {}
void GraphicsSystem::visit(Gamepad*) {}
