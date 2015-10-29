#ifndef GAME_H
#define GAME_H

#include "dependencies.h"
#include "Util/FSM.h"
#include "Command/CloseCommand.h"
#include "Command/KeyPressCommand.h"
#include "Command/WindowResizeCommand.h"
#include "Command/MouseMoveCommand.h"
#include "Entities/ScreenWriter.h"
#include "Entities/Mouse.h"
#include "Util/InputListener.h"

class Game
: public HasState
, public InputListener 
{
public:
   enum INPUT {
      NOP = 0,
      ENTER,
      SPACE,
      ESC
   };
   
	Game();
	~Game();

	// helper functions
	void process_event();

	ScreenWriter sw;
	sf::RenderWindow window;

   // game states
   void start_menu();
   void map_menu();
   void builder(); 

   void reset();

   // command interface
   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);

protected:
   std::map<std::string, sf::View*> views;
   
   // state contexts (need to clean this up)
   sf::Vector2f origin;
   
   Mouse* m;
};

#endif
