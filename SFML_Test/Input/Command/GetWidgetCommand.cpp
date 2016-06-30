#include "GetWidgetCommand.h"
#include "SceneGraphNode.h"
#include "Camera.h"
#include "Draw.h"
#include "Entity.h"
#include "Widget.h"
#include "PlayerGamepad.h"

GetWidgetCommand::GetWidgetCommand(Scene::SceneGraph* scene_graph, PlayerGamepad* gamepad /* = nullptr */)
: scene_graph_(scene_graph)
, gamepad_(gamepad)
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

void GetWidgetCommand::gamepad(PlayerGamepad* gamepad) {
   this->gamepad_ = gamepad;
}

PlayerGamepad* GetWidgetCommand::gamepad() {
   return this->gamepad_;
}

const WidgetList& GetWidgetCommand::get() {
   return this->widgets_;
}

void GetWidgetCommand::execute() {
   this->widgets_.clear();

   if (!this->gamepad_) {
      Service::get_logger().msg("GetWidgetCommand", Logger::WARNING, "Command has no gamepad reference.");
      return;
   }

   Service::get_logger().msg("GetWidgetCommand", Logger::INFO, "target = (" + std::to_string(this->gamepad_->position().x) + ", " + std::to_string(this->gamepad_->position().y) + ")");

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

   Service::get_logger().msg("GetWidgetCommand", Logger::INFO, "GOT WIDGETS");
   for (WidgetList::const_iterator wit = this->widgets_.begin(); wit != this->widgets_.end(); ++wit) {
      Service::get_logger().msg("GetWidgetCommand", Logger::INFO, **wit);
   }
}

void GetWidgetCommand::unexecute() {
   this->widgets_.clear();
}

void GetWidgetCommand::visit(Camera* camera) {}
void GetWidgetCommand::visit(Draw* draw) {}
void GetWidgetCommand::visit(Entity* entity) {}

void GetWidgetCommand::visit(Widget* widget) {
   if (widget && widget->intersects(this->gamepad_->position())) {
      this->widgets_.push_back(widget);
   }
}
