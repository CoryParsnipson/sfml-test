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
#include "Entities/Mouse.h"
#include "Viewport.h"
#include "Util/InputListener.h"
#include "Util/Graphics.h"
#include "Service.h"

#include "FullLogger.h"

#include "Manager/TextureManager.h"

#include "GameState.h"

class Game
: public InputListener
{
public:
	Game();
	virtual ~Game();

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
   Graphics graphics;

   //Mouse* m;

   TextureManager texture_manager;

protected:
   GameState* state_;

   FullLogger full_logger_;
   InputController input_;
};

#endif
