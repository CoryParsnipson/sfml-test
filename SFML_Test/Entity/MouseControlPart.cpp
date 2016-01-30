#include "MouseControlPart.h"

#include "Game.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"

#include "MouseMoveCommand.h"
#include "MouseWheelCommand.h"
#include "MouseButtonCommand.h"

MouseControlPart::MouseControlPart(std::string id)
: ControlPart(id)
, is_panning(false)
, zoom_delta(0)
, controllable_(nullptr)
, mouse_buttons_(nullptr)
{
   Service::get_logger().msg("ControlPart", Logger::INFO, "Creating MouseControlPart '" + id + "'");
}

MouseControlPart::~MouseControlPart() {
   this->controllable_ = nullptr; // don't delete controllable, no ownership of this

   delete this->mouse_buttons_;
   this->mouse_buttons_ = nullptr;
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

   delete this->mouse_buttons_;
   this->mouse_buttons_ = new MouseButtonCommand(c);

   this->is_panning = (c.state == MouseButtonCommand::PRESSED);
   this->panning_anchor = sf::Vector2f(c.x, c.y);

   switch (c.button) {
   case MouseButtonCommand::LEFT:
      this->controllable_->click(c);
      break;
   default: break;
   }
}

void MouseControlPart::process(MouseWheelCommand& c) {
   this->zoom_delta = c.delta;
}

void MouseControlPart::set_controllable(MouseControllable* c) {
   this->controllable_ = c;
}

void MouseControlPart::update(Game& game, Scene* scene, Entity* entity) {
   if (!entity) {
      Service::get_logger().msg("MouseControlPart", Logger::ERROR, "Entity null pointer received.");
      return;
   }
   
   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity->get("physics")); // is there a better way?
   if (physics) {
      physics->set_position(this->last_mouse_pos);
   }

   if (!this->controllable_) {
      Service::get_logger().msg("MouseControlPart", Logger::WARNING, "Controllable target not registered!");
      return;
   }

   // if we are in panning state, move the viewport
   if (this->is_panning) {
      sf::Vector2f panning_delta = (this->panning_anchor - this->last_mouse_pos) * this->controllable_->get_scale();

      // TODO: update mouse button command (note, you should refactor this entire thing...)
      this->mouse_buttons_->x = this->last_mouse_pos.x;
      this->mouse_buttons_->y = this->last_mouse_pos.y;

      this->controllable_->drag(*this->mouse_buttons_, this->MOUSE_PAN_COEFFICIENT * panning_delta);
      
      this->panning_anchor = this->last_mouse_pos;
   }

   // respond to mouse wheel events
   if (this->zoom_delta) {
      this->controllable_->set_scale(this->controllable_->get_scale() - this->zoom_delta / MouseControlPart::WINDOW_RESIZE_COEFFICIENT);
      this->zoom_delta = 0;
   }
}
