#include "MouseControlPart.h"

#include "Entity2.h"
#include "PhysicsPart.h"

#include "MouseMoveCommand.h"
#include "MouseButtonCommand.h"

MouseControlPart::MouseControlPart(std::string id)
: ControlPart(id)
, is_panning(false)
{
   Service::get_logger().msg("ControlPart", Logger::INFO, "Creating MouseControlPart '" + id + "'");
}

MouseControlPart::~MouseControlPart() {
}

void MouseControlPart::process(CloseCommand& c) {}
void MouseControlPart::process(KeyPressCommand& c) {}
void MouseControlPart::process(WindowResizeCommand& c) {}

void MouseControlPart::process(MouseMoveCommand& c) {
   // update the last seen mouse position
   this->last_mouse_pos.x = c.x;
   this->last_mouse_pos.y = c.y;
}

void MouseControlPart::process(MouseButtonCommand& c) {
   Service::get_logger().msg("ControlPart", Logger::INFO, c.to_string());

   switch (c.button) {
   case MouseButtonCommand::RIGHT:
      Service::get_logger().msg("ControlPart", Logger::INFO, "is panning = " + std::to_string(c.state == MouseButtonCommand::PRESSED));      

      this->is_panning = (c.state == MouseButtonCommand::PRESSED);
      this->panning_anchor = sf::Vector2f(c.x, c.y);
      break;
   default: break;
   }
}

void MouseControlPart::process(MouseWheelCommand& c) {
}

void MouseControlPart::update(Entity2& entity, Viewport& viewport) {
   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity.get("physics")); // is there a better way?
   if (physics) {
      physics->set_position(this->last_mouse_pos);
   }

   // if we are in panning state, move the viewport
   if (this->is_panning) {
      sf::Vector2f panning_delta = (this->panning_anchor - this->last_mouse_pos) * viewport.get_zoom_factor();
      viewport.move(this->MOUSE_PAN_COEFFICIENT * panning_delta);
      
      this->panning_anchor = this->last_mouse_pos;
   }
}
