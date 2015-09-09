#ifndef GAME_H
#define GAME_H

#include "dependencies.h"
#include "Util/FSM.h"
#include "Command/CloseCommand.h"
#include "Entities/GameEntity.h"
#include "Entities/ScreenWriter.h"

class Game
: public HasState
, public GameEntity
{
public:
   enum INPUT {
      NOP,
      SPACE,
      ENTER,
      ESC
   };

	Game();
	~Game();

	// helper functions
	void process_event();

   // game states
   void start_menu();
   void map_menu();
   void builder(); 

   void reset();

	ScreenWriter sw;
	sf::RenderWindow window;

   // command interface
   virtual void process(CloseCommand& c);
};

#endif
