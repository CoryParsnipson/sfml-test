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

#include "Viewport.h"
#include "Entity.h"

// forward declarations
class Game;

// ----------------------------------------------------------------------------
// class Mouse
// Wrapper class for SFML Mouse. Contains state information for click and drag
// etc.
// ----------------------------------------------------------------------------
class Mouse
: public InputListener
, public Entity
{
public:
	const unsigned int MOUSE_HISTORY_LENGTH = 10;
	const float MOUSE_PAN_COEFFICIENT = 1.f;
	const float WINDOW_RESIZE_COEFFICIENT = 10.f;

	Mouse(GraphicsComponent* graphics);

	sf::Vector2i get_mouse_position();
   void set_view(Viewport* v);
  
   // entity interface
   virtual void update(Viewport& viewport);

   // command interface
   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);
   virtual void process(MouseButtonCommand& c);
   virtual void process(MouseWheelCommand& c);

protected:
   Viewport* viewport;

	bool is_panning;
	sf::Vector2i panning_anchor;

	std::list<sf::Vector2i> last_positions;	
};

#endif
