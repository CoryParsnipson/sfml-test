#include "MouseControlPart.h"

#include "Entity2.h"
#include "PhysicsPart.h"

#include "MouseMoveCommand.h"

MouseControlPart::MouseControlPart(std::string id)
: ControlPart(id)
{
   Service::get_logger().msg("ControlPart", Logger::INFO, "Creating MouseControlPart '" + id + "'");
}

MouseControlPart::~MouseControlPart() {
}

void MouseControlPart::process(CloseCommand& c) {
}

void MouseControlPart::process(KeyPressCommand& c) {
}

void MouseControlPart::process(WindowResizeCommand& c) {
}

void MouseControlPart::process(MouseMoveCommand& c) {
   this->last_mouse_pos.x = c.x;
   this->last_mouse_pos.y = c.y;
}

void MouseControlPart::process(MouseButtonCommand& c) {
}

void MouseControlPart::process(MouseWheelCommand& c) {
}

void MouseControlPart::update(Entity2& entity, Viewport& viewport) {
   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity.get("physics")); // is there a better way?
   if (physics) {
      physics->set_position(this->last_mouse_pos);
   }
}
