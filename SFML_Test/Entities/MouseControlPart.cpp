#include "MouseControlPart.h"

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
}

void MouseControlPart::process(MouseButtonCommand& c) {
   Service::get_logger().msg("ControlPart", Logger::INFO, "PENIS");
}

void MouseControlPart::process(MouseWheelCommand& c) {
}

void MouseControlPart::update(Entity2& entity, Viewport& viewport) {
}
