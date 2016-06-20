#include "GetWidgetCommand.h"
#include "SceneGraphNode.h"
#include "Camera.h"
#include "Draw.h"
#include "Entity.h"
#include "Widget.h"

GetWidgetCommand::GetWidgetCommand(Scene::SceneGraph* scene_graph, sf::Vector2f target /* = sf::Vector2f(0, 0) */ )
: target_(target)
, scene_graph_(scene_graph)
{
}

GetWidgetCommand::~GetWidgetCommand() {
   this->widgets_.clear();
}

void GetWidgetCommand::scene_graph(Scene::SceneGraph* scene_graph) {
   this->scene_graph_ = scene_graph;
}

Scene::SceneGraph* GetWidgetCommand::scene_graph() {
   return this->scene_graph_;
}

void GetWidgetCommand::target(sf::Vector2f pos) {
   this->target_ = pos;
}

const sf::Vector2f& GetWidgetCommand::target() {
   return this->target_;
}

const WidgetList& GetWidgetCommand::get() {
   return this->widgets_;
}

void GetWidgetCommand::execute() {
   this->widgets_.clear();

   // iterate from highest z-index to lowest
   Scene::SceneGraph::const_reverse_iterator it;
   for (it = this->scene_graph_->rbegin(); it != this->scene_graph_->rend(); ++it) {
      SceneGraphNode::postfix_iterator node_it;
      for (node_it = it->second->begin(); node_it != it->second->end(); ++node_it) {
         if (!(*node_it)->visible()) {
            continue;
         }
         (*node_it)->accept(*this);
      }
   }
}

void GetWidgetCommand::unexecute() {
   this->widgets_.clear();
}

void GetWidgetCommand::visit(Camera* camera) {}
void GetWidgetCommand::visit(Draw* draw) {}
void GetWidgetCommand::visit(Entity* entity) {}

void GetWidgetCommand::visit(Widget* widget) {
   if (widget && widget->intersects(this->target_)) {
      this->widgets_.push_back(widget);
   }
}
