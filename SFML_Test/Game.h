#ifndef GAME_H
#define GAME_H

#include "dependencies.h"
#include "Util/FSM.h"
#include "Command/CloseCommand.h"
#include "Command/KeyPressCommand.h"
#include "Command/WindowResizeCommand.h"
#include "Command/MouseMoveCommand.h"
#include "Command/MouseButtonCommand.h"
#include "Command/MouseWheelCommand.h"
#include "Entities/ScreenWriter.h"
#include "Entities/Mouse.h"
#include "Entities/Viewport.h"
#include "Util/InputListener.h"

#include "GameState.h"

class Game
: public InputListener
{
public:
	Game();
	~Game();

	// helper functions
	void main_loop();
   void reset();
   void exit();

   // command interface
   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);
   virtual void process(MouseButtonCommand& c);
   virtual void process(MouseWheelCommand& c);

   // game functional units 
	sf::RenderWindow window;

	ScreenWriter sw;
   Mouse* m;

protected:
   GameState* state_;
};

#endif
