#include "GetSceneObjectCommand.h"
#include "Camera.h"
#include "Entity.h"
#include "Widget.h"

GetSceneObjectCommand::GetSceneObjectCommand(SceneObject* scene_graph, sf::Vector2f target /* = sf::Vector2f(0, 0) */)
: Command("GetSceneObjectCommand")
, target_(target)
, scene_graph_(scene_graph)
{
}

GetSceneObjectCommand::~GetSceneObjectCommand() {
   this->clear_lists();
}

void GetSceneObjectCommand::scene_graph(SceneObject* scene_graph) {
   this->scene_graph_ = scene_graph;
}

SceneObject* GetSceneObjectCommand::scene_graph() {
   return this->scene_graph_;
}

const SceneObjectList& GetSceneObjectCommand::get() {
   return this->scene_objects_;
}

const WidgetList& GetSceneObjectCommand::get_widgets() {
   return this->widgets_;
}

const CameraList& GetSceneObjectCommand::get_cameras() {
   return this->cameras_;
}

void GetSceneObjectCommand::target(sf::Vector2f target) {
   this->target_ = target;
}

sf::Vector2f GetSceneObjectCommand::target() {
   return this->target_;
}

void GetSceneObjectCommand::clear_lists() {
   this->cameras_.clear();
   this->entities_.clear();
   this->widgets_.clear();

   this->scene_objects_.clear();
}

void GetSceneObjectCommand::execute() {
   Service::get_logger().msg("GetSceneObjectCommand", Logger::INFO, "target = (" + std::to_string(this->target_.x) + ", " + std::to_string(this->target_.y) + ")");
   this->clear_lists();

   // iterate from highest z-index to lowest
   SceneObject::reverse_postfix_iterator it;
   for (it = this->scene_graph_->rbegin(); it != this->scene_graph_->rend(); ++it) {
      if (!(*it)->visible()) {
         continue;
      }
      (*it)->accept(*this);
   }
}

void GetSceneObjectCommand::unexecute() {
   this->clear_lists();
}

void GetSceneObjectCommand::visit(Camera* camera) {
   if (camera && camera->intersects(this->target_)) {
      this->cameras_.push_back(camera);
      this->scene_objects_.push_back(camera);
   }
}

void GetSceneObjectCommand::visit(Entity* entity) {
   if (entity && entity->intersects(this->target_)) {
      this->entities_.push_back(entity);
      this->scene_objects_.push_back(entity);
   }
}

void GetSceneObjectCommand::visit(Map* map) {
   return;
}

void GetSceneObjectCommand::visit(Grid* grid) {
   return;
}

void GetSceneObjectCommand::visit(Widget* widget) {
   if (widget && widget->intersects(this->target_)) {
      this->widgets_.push_back(widget);
      this->scene_objects_.push_back(widget);

      Service::get_logger().msg("GetSceneObjectCommand", Logger::INFO, "Got widget: " + std::string(*widget));
   }
}

void GetSceneObjectCommand::visit(Gamepad* gamepad) {
   if (gamepad && gamepad->intersects(this->target_)) {
      this->gamepads_.push_back(gamepad);
      this->scene_objects_.push_back(gamepad);
   }
}
