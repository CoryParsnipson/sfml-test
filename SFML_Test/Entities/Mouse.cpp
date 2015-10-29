#include "Mouse.h"

Mouse::Mouse(sf::RenderWindow& window, sf::View& view)
: window(window)
, view(view)
, zoom_factor(1.0f)
, is_panning(false)
, cursor(sf::Vector2f(6, 6))
{
	cursor.setFillColor(sf::Color::Red);
	cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(this->window)));
}

void Mouse::process(CloseCommand& c) { }
void Mouse::process(KeyPressCommand& c) { }
void Mouse::process(WindowResizeCommand& c) { }

void Mouse::process(MouseMoveCommand& c) {
   sf::Vector2i pos;
   sf::Vector2f panning_delta;

	// update the red dot on the screen to the current position
	this->cursor.setPosition(static_cast<sf::Vector2f>(this->get_mouse_position()));

	if (!this->is_panning) {
		return;
	}
	pos = this->get_mouse_position();

	panning_delta = static_cast<sf::Vector2f>(this->panning_anchor - pos) * this->zoom_factor;
	this->view.move(this->MOUSE_PAN_COEFFICIENT * panning_delta);

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

void Mouse::process_event(sf::Event& event) {
   sf::Vector2i pos;
   sf::Vector2f panning_delta;

	switch (event.type) {
	case sf::Event::Resized:
		break;
	case sf::Event::MouseButtonPressed:
		switch (event.mouseButton.button) {
		case sf::Mouse::Left:
			break;
		case sf::Mouse::Right:
			this->is_panning = true;
			this->panning_anchor = this->get_mouse_position();
			break;
		case sf::Mouse::Middle:
			break;
      default:
         break;
		}
		break;
	case sf::Event::MouseButtonReleased:
		switch (event.mouseButton.button) {
		case sf::Mouse::Left:
			break;
		case sf::Mouse::Right:
			this->is_panning = false;
			this->panning_anchor = this->get_mouse_position();
			break;
		case sf::Mouse::Middle:
			break;
      default:
         break;
		}
		break;
	case sf::Event::MouseMoved:
		// update the red dot on the screen to the current position
		this->cursor.setPosition(static_cast<sf::Vector2f>(this->get_mouse_position()));

		if (!this->is_panning) {
			return;
		}
		pos = this->get_mouse_position();

		panning_delta = static_cast<sf::Vector2f>(this->panning_anchor - pos) * this->zoom_factor;
		this->view.move(this->MOUSE_PAN_COEFFICIENT * panning_delta);

		this->panning_anchor = pos;
		break;
   case sf::Event::MouseWheelMoved:
      this->set_zoom_factor(this->get_zoom_factor() + event.mouseWheel.delta / 10.0f);
      this->view.setSize(sf::Vector2f(this->get_zoom_factor() * this->window.getSize().x, this->get_zoom_factor() * this->window.getSize().y));
      break;
   default:
      break;
	}
}

void Mouse::add_mouse_position(int x, int y) {
	this->last_positions.push_front(sf::Vector2i(x, y));

	if (this->last_positions.size() > this->MOUSE_HISTORY_LENGTH) {
		this->last_positions.pop_back();
	}
}

sf::Vector2i Mouse::get_mouse_position() {
	return sf::Mouse::getPosition(this->window);
}

sf::Vector2i Mouse::get_last_mouse_position(unsigned int frames_to_go_back /* = 0 */) {
	if (frames_to_go_back >= this->last_positions.size()) {
		return sf::Vector2i(0, 0);
	}

	std::list<sf::Vector2i>::iterator it = this->last_positions.begin();
	for (unsigned int i = 0; it != this->last_positions.end(), i < frames_to_go_back; it++, i++) {}

	return sf::Vector2i(it->x, it->y);
}

void Mouse::set_zoom_factor(float zf) {
   float zoom_factor = zf;

   if (zoom_factor < 0.125) {
      zoom_factor = 0.125;
   }
   else if (zoom_factor > 3.0) {
      zoom_factor = 3.0;
   }

   this->zoom_factor = zoom_factor;
}

float Mouse::get_zoom_factor() {
   return this->zoom_factor;
}

void Mouse::draw(sf::RenderWindow& window) {
	window.draw(this->cursor);
}

void Mouse::draw(sf::RenderWindow& window, sf::View& view) {
	sf::View old_view = window.getView();

	window.setView(view);
	this->draw(window);
	window.setView(old_view);
}

sf::RectangleShape& Mouse::get_cursor() {
   return this->cursor;
}
