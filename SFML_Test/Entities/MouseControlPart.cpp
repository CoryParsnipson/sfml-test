#include "MouseControlPart.h"

#include "Game.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"

#include "MouseMoveCommand.h"
#include "MouseWheelCommand.h"
#include "MouseButtonCommand.h"

MouseControlPart::MouseControlPart(std::string id, Viewport* viewport)
: ControlPart(id)
, is_panning(false)
, zoom_delta(0)
, viewport_(viewport)
, last_click_pos(nullptr)
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
   case MouseButtonCommand::LEFT:
      this->last_click_pos = new sf::Vector2f(c.x, c.y);
      break;
   case MouseButtonCommand::RIGHT:
      this->is_panning = (c.state == MouseButtonCommand::PRESSED);
      this->panning_anchor = sf::Vector2f(c.x, c.y);
      break;
   default: break;
   }
}

void MouseControlPart::process(MouseWheelCommand& c) {
   this->zoom_delta = c.delta;
}

void MouseControlPart::update(Entity& entity, Game& game) {
   if (!this->viewport_) {
      return;
   }

   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity.get("physics")); // is there a better way?
   if (physics) {
      physics->set_position(this->last_mouse_pos);
   }

   // if we are in panning state, move the viewport
   if (this->is_panning) {
      sf::Vector2f panning_delta = (this->panning_anchor - this->last_mouse_pos) * this->viewport_->get_zoom_factor();
      this->viewport_->move(this->MOUSE_PAN_COEFFICIENT * panning_delta);
      
      this->panning_anchor = this->last_mouse_pos;
   }

   // respond to mouse wheel events
   if (this->zoom_delta) {
      this->viewport_->set_zoom_factor(this->viewport_->get_zoom_factor() - this->zoom_delta / MouseControlPart::WINDOW_RESIZE_COEFFICIENT);
      this->zoom_delta = 0;
   }

   // handle mouse click positions
   if (this->last_click_pos) {
      Service::get_logger().msg("MouseControlPart", Logger::INFO, "LAST CLICK    : (" + std::to_string(this->last_click_pos->x) + ", " + std::to_string(this->last_click_pos->y) + ")");
      sf::Vector2f world_coord = this->viewport_->get_world_coord(static_cast<sf::Vector2i>(*this->last_click_pos));
      Service::get_logger().msg("MouseControlPart", Logger::INFO, "LAST CLICK NOF: (" + std::to_string(world_coord.x) + ", " + std::to_string(world_coord.y) + ")");

      delete this->last_click_pos;
      this->last_click_pos = nullptr;
   } 
}
