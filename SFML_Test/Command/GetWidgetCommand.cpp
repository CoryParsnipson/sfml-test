#include "GetWidgetCommand.h"
#include "SceneGraphNode.h"
#include "Camera.h"
#include "Draw.h"
#include "Entity.h"
#include "Widget.h"

GetWidgetCommand::GetWidgetCommand(SceneGraphNode* scene_graph, sf::Vector2f target /* = sf::Vector2f(0, 0) */)
: Command("GetWidgetCommand")
, target_(target)
, scene_graph_(scene_graph)
{
}

GetWidgetCommand::~GetWidgetCommand() {
   this->widgets_.clear();
}

void GetWidgetCommand::scene_graph(SceneGraphNode* scene_graph) {
   this->scene_graph_ = scene_graph;
}

SceneGraphNode* GetWidgetCommand::scene_graph() {
   return this->scene_graph_;
}

const WidgetList& GetWidgetCommand::get() {
   return this->widgets_;
}

void GetWidgetCommand::target(sf::Vector2f target) {
   this->target_ = target;
}

sf::Vector2f GetWidgetCommand::target() {
   return this->target_;
}

void GetWidgetCommand::execute() {
   this->widgets_.clear();

   Service::get_logger().msg("GetWidgetCommand", Logger::INFO, "target = (" + std::to_string(this->target_.x) + ", " + std::to_string(this->target_.y) + ")");

   // iterate from highest z-index to lowest
   SceneGraphNode::reverse_postfix_iterator it;
   for (it = this->scene_graph_->rbegin(); it != this->scene_graph_->rend(); ++it) {
      if (!(*it)->visible()) {
         continue;
      }
      (*it)->accept(*this);
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
   if (widget && widget->intersects(this->target_)) {
      this->widgets_.push_back(widget);
   }
}
