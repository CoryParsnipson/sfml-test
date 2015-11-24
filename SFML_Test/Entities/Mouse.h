#ifndef MOUSE_H
#define MOUSE_H

#include "../dependencies.h"
#include "Util/InputListener.h"

#include "Command/CloseCommand.h"
#include "Command/KeyPressCommand.h"
#include "Command/WindowResizeCommand.h"
#include "Command/MouseMoveCommand.h"
#include "Command/MouseButtonCommand.h"
#include "Command/MouseWheelCommand.h"

#include "Entities/Viewport.h"

#include "Graphics.h"

// forward declarations
class Game;

// ----------------------------------------------------------------------------
// class Mouse
// Wrapper class for SFML Mouse. Contains state information for click and drag
// etc.
// ----------------------------------------------------------------------------
class Mouse
: public InputListener
{
public:
	const unsigned int MOUSE_HISTORY_LENGTH = 10;
	const float MOUSE_PAN_COEFFICIENT = 1.f;
	const float WINDOW_RESIZE_COEFFICIENT = 10.f;

	Mouse(Game& game);

	void process_event(sf::Event& event);
	void add_mouse_position(int x, int y);
	sf::Vector2i get_mouse_position(Graphics* graphics = nullptr);
	sf::Vector2i get_last_mouse_position(unsigned int frames_to_go_back = 0);

	void draw(Viewport& viewport);

   sf::RectangleShape& get_cursor();
   
   void set_view(Viewport* v);
  
   // command interface
   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);
   virtual void process(MouseButtonCommand& c);
   virtual void process(MouseWheelCommand& c);

protected:
   Game& game;
   Viewport* view;

	bool is_panning;
	sf::Vector2i panning_anchor;

	sf::RectangleShape cursor;
	std::list<sf::Vector2i> last_positions;	
};

#endif
