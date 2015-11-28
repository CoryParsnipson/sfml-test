#include "Mouse.h"
#include "Game.h"

Mouse::Mouse(GraphicsComponent* graphics)
: Entity(graphics)
, is_panning(false)
{
}

void Mouse::update(Viewport& viewport) {
   this->graphics_->update(this, viewport);
}

void Mouse::process(CloseCommand& c) { }
void Mouse::process(KeyPressCommand& c) { }
void Mouse::process(WindowResizeCommand& c) { }

void Mouse::process(MouseMoveCommand& c) {
   sf::Vector2i pos;
   sf::Vector2f panning_delta;

	if (!this->is_panning) {
		return;
	}
	pos = this->get_mouse_position();

	panning_delta = static_cast<sf::Vector2f>(this->panning_anchor - pos) * this->viewport->get_zoom_factor();
   if (this->viewport) {
	   this->viewport->move(this->MOUSE_PAN_COEFFICIENT * panning_delta);
   } 

	this->panning_anchor = pos;
}

void Mouse::process(MouseButtonCommand& c) {
   switch (c.button) {
   case MouseButtonCommand::LEFT: break;
   case MouseButtonCommand::RIGHT:
      this->is_panning = (c.state == MouseButtonCommand::PRESSED);
      this->panning_anchor = this->get_mouse_position();
      break;
   case MouseButtonCommand::MIDDLE: break;
   default: break;
   }
}

void Mouse::process(MouseWheelCommand& c) {
   if (this->viewport) {
      this->viewport->set_zoom_factor(this->viewport->get_zoom_factor() - c.delta / Mouse::WINDOW_RESIZE_COEFFICIENT);
   }
}

sf::Vector2i Mouse::get_mouse_position() {
   if (this->viewport) {
	   return sf::Mouse::getPosition(this->viewport->get_graphics().get_window());
   }

	return sf::Mouse::getPosition();
}

void Mouse::set_view(Viewport* v) {
   this->viewport = v;
}
