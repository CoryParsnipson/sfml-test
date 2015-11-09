#ifndef GAME_STARTMENU_STATE_H
#define GAME_STARTMENU_STATE_H

#include "dependencies.h"

#include "GameState.h"

// forward declarations
class Game;

class GameStartMenuState : public GameState {
public:
   GameStartMenuState();
   ~GameStartMenuState();

   virtual void enter(Game& game);
   virtual void exit(Game& game);

   virtual GameState* update(Game& game);

   // command interface
   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);
   virtual void process(MouseButtonCommand& c);
   virtual void process(MouseWheelCommand& c);

protected:
   bool is_running;
};

#endif
