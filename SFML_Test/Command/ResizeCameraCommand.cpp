#include "ResizeCameraCommand.h"
#include "SceneObject.h"
#include "Camera.h"

ResizeCameraCommand::ResizeCameraCommand(SceneObject* scene_graph)
: scene_graph_(scene_graph)
{
}

ResizeCameraCommand::~ResizeCameraCommand() {
}

void ResizeCameraCommand::execute() {
   if (!this->scene_graph_) {
      return;
   }

   SceneObject::prefix_iterator it = this->scene_graph_->begin();
   for (; it != this->scene_graph_->end(); ++it) {
      it->accept(*this);
   }
}

void ResizeCameraCommand::unexecute() {
   // TODO: implement me
}

void ResizeCameraCommand::visit(Camera* camera) {
   if (camera) {
      camera->resize();
   }
}

void ResizeCameraCommand::visit(Entity* entity) {}
void ResizeCameraCommand::visit(Map* map) {}
void ResizeCameraCommand::visit(Grid* grid) {}
void ResizeCameraCommand::visit(Widget* widget) {}
void ResizeCameraCommand::visit(Gamepad* gamepad) {}
