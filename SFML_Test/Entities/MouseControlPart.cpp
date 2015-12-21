#include "MouseControlPart.h"

MouseControlPart::MouseControlPart() {
   Service::get_logger().msg("ControlPart", Logger::INFO, "Creating MouseControlPart");
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
}

void MouseControlPart::process(MouseWheelCommand& c) {
}

void MouseControlPart::update(Entity2& entity, Viewport& viewport) {
}
