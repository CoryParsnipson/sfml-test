#include "GetWidgetCommand.h"
#include "SceneGraphNode.h"
#include "Camera.h"
#include "Draw.h"
#include "Entity.h"
#include "Widget.h"
#include "Gamepad.h"

GetWidgetCommand::GetWidgetCommand(Scene::SceneGraph* scene_graph, Gamepad* gamepad)
: Command("GetWidgetCommand")
, scene_graph_(scene_graph)
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

void GetWidgetCommand::gamepad(Gamepad* gamepad) {
   this->gamepad_ = gamepad;
}

Gamepad* GetWidgetCommand::gamepad() {
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

   sf::Vector2f gamepad_cursor(this->gamepad_->cursor_position());
   Service::get_logger().msg("GetWidgetCommand", Logger::INFO, "target = (" + std::to_string(gamepad_cursor.x) + ", " + std::to_string(gamepad_cursor.y) + ")");

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
   if (widget && widget->intersects(this->gamepad_->cursor_position())) {
      this->widgets_.push_back(widget);
   }
}
