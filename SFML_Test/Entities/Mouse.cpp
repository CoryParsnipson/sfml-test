#include "Mouse.h"

#include "Game.h"

Mouse::Mouse(Game& game)
: game(game)
, is_panning(false)
, cursor(sf::Vector2f(6, 6))
{
	cursor.setFillColor(sf::Color::Red);
   cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(game.graphics.get_window())));
}

void Mouse::process(CloseCommand& c) { }
void Mouse::process(KeyPressCommand& c) { }
void Mouse::process(WindowResizeCommand& c) { }

void Mouse::process(MouseMoveCommand& c) {
   sf::Vector2i pos;
   sf::Vector2f panning_delta;

	// update the red dot on the screen to the current position
	this->cursor.setPosition(static_cast<sf::Vector2f>(this->get_mouse_position(&this->game.graphics)));

	if (!this->is_panning) {
		return;
	}
	pos = this->get_mouse_position(&this->game.graphics);

	panning_delta = static_cast<sf::Vector2f>(this->panning_anchor - pos) * this->view->get_zoom_factor();
   if (this->view) {
	   this->view->move(this->MOUSE_PAN_COEFFICIENT * panning_delta);
   } 

	this->panning_anchor = pos;
}

void Mouse::process(MouseButtonCommand& c) {
   switch (c.button) {
   case MouseButtonCommand::LEFT: break;
   case MouseButtonCommand::RIGHT:
      this->is_panning = (c.state == MouseButtonCommand::PRESSED);
      this->panning_anchor = this->get_mouse_position(&this->game.graphics);
      break;
   case MouseButtonCommand::MIDDLE: break;
   default: break;
   }
}

void Mouse::process(MouseWheelCommand& c) {
   if (this->view) {
      this->view->set_zoom_factor(this->view->get_zoom_factor() - c.delta / Mouse::WINDOW_RESIZE_COEFFICIENT);
   }
}

void Mouse::add_mouse_position(int x, int y) {
	this->last_positions.push_front(sf::Vector2i(x, y));

	if (this->last_positions.size() > this->MOUSE_HISTORY_LENGTH) {
		this->last_positions.pop_back();
	}
}

sf::Vector2i Mouse::get_mouse_position(Graphics* graphics /* = nullptr */) {
   if (graphics) {
      return sf::Mouse::getPosition(graphics->get_window());
   }

	return sf::Mouse::getPosition();
}

sf::Vector2i Mouse::get_last_mouse_position(unsigned int frames_to_go_back /* = 0 */) {
	if (frames_to_go_back >= this->last_positions.size()) {
		return sf::Vector2i(0, 0);
	}

	std::list<sf::Vector2i>::iterator it = this->last_positions.begin();
	for (unsigned int i = 0; it != this->last_positions.end(), i < frames_to_go_back; it++, i++) {}

	return sf::Vector2i(it->x, it->y);
}

void Mouse::draw(Viewport& viewport) {
   viewport.draw(this->cursor);
}

sf::RectangleShape& Mouse::get_cursor() {
   return this->cursor;
}

void Mouse::set_view(Viewport* v) {
   this->view = v;
}
