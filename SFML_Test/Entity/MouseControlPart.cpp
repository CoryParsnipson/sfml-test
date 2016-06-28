#include "Game.h"
#include "MouseControlPart.h"

#include "Entity.h"
#include "PhysicsPart.h"

MouseControlPart::MouseControlPart(std::string id)
: ControlPart(id)
, is_panning(false)
, zoom_delta(0)
, controllable_(nullptr)
{
   Service::get_logger().msg("ControlPart", Logger::INFO, "Creating MouseControlPart '" + id + "'");
}

MouseControlPart::~MouseControlPart() {
   this->controllable_ = nullptr; // don't delete controllable, no ownership of this
}

void MouseControlPart::process(CloseInputEvent& e) {}
void MouseControlPart::process(ResizeInputEvent& e) {}
void MouseControlPart::process(KeyPressInputEvent& e) {}

void MouseControlPart::process(MouseMoveInputEvent& e) {
   // update the last seen mouse position
   this->last_mouse_pos.x = e.x;
   this->last_mouse_pos.y = e.y;
}

void MouseControlPart::process(MouseWheelInputEvent& e) {
   this->zoom_delta = e.delta;
}

void MouseControlPart::process(MouseButtonInputEvent& e) {
   Service::get_logger().msg("ControlPart", Logger::INFO, e);
   
   this->button_ = e.button;
   this->is_panning = (e.state == ButtonState::Pressed);
   this->panning_anchor = sf::Vector2f(e.x, e.y);

   if (this->controllable_) {
      this->controllable_->click(e.button, e.state, this->panning_anchor);
   } else {
      Service::get_logger().msg("ControlPart", Logger::WARNING, "Controllable pointer is null.");
   }
}

void MouseControlPart::set_controllable(MouseControllable* c) {
   this->controllable_ = c;
}

void MouseControlPart::update(Game& game, Scene* scene, Entity* entity) {
   if (!entity) {
      Service::get_logger().msg("MouseControlPart", Logger::ERROR, "Entity null pointer received.");
      return;
   }

   entity->set_position(this->last_mouse_pos);

   if (!this->controllable_) {
      Service::get_logger().msg("MouseControlPart", Logger::WARNING, "Controllable target not registered!");
      return;
   }

   // if we are in panning state, move the viewport
   if (this->is_panning) {
      sf::Vector2f panning_delta = (this->last_mouse_pos - this->panning_anchor) * this->controllable_->get_scale();

      this->panning_anchor = this->last_mouse_pos;
      this->controllable_->drag(this->button_, this->last_mouse_pos, this->MOUSE_PAN_COEFFICIENT * panning_delta);
   }

   // respond to mouse wheel events
   if (this->zoom_delta) {
      this->controllable_->set_scale(this->controllable_->get_scale() - this->zoom_delta / MouseControlPart::WINDOW_RESIZE_COEFFICIENT);
      this->zoom_delta = 0;
   }
}

Serialize::SerialObj MouseControlPart::serialize() {
   Serialize::SerialObj obj;
   return obj;
}

void MouseControlPart::deserialize(Serialize::SerialObj& obj) {}
