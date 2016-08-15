#include "RemoveCommand.h"
#include "SceneObject.h"

RemoveCommand::RemoveCommand(SceneObject* scene_graph, SceneObject* scene_object)
: Command("RemoveCommand")
, scene_object_(scene_object)
, scene_graph_(scene_graph)
, child_idx_(0)
, parent_node_(nullptr)
{
}

RemoveCommand::~RemoveCommand() {
}

void RemoveCommand::execute() {
   if (!this->scene_object_ || !this->scene_graph_) {
      return;
   }

   SceneObject::prefix_iterator it;
   for (it = this->scene_graph_->begin(); it != this->scene_graph_->end(); ++it) {
      if (this->scene_object_ == *it) {
         this->parent_node_ = this->scene_object_->parent();

         if (this->parent_node_) {
            // find child id of scene object node
            SceneObject::iterator p_it;
            for (p_it = this->parent_node_->begin(); p_it != this->parent_node_->end(); ++p_it) {
               if (*p_it == this->scene_object_) {
                  return;
               }
               ++this->child_idx_;
            }

            this->parent_node_->remove(this->scene_object_);
         } else {
            Service::get_logger().msg(this->id_, Logger::WARNING, "Will not remove scene_graph root node.");
         }
         return;
      }
   }
}

void RemoveCommand::unexecute() {
   if (!this->parent_node_ || !this->scene_graph_ || !this->scene_object_) {
      return;
   }

   this->parent_node_->insert(this->child_idx_, this->scene_object_);
}

SceneObject* RemoveCommand::scene_object() {
   return this->scene_object_;
}

SceneObject* RemoveCommand::parent() {
   return this->parent_node_;
}
