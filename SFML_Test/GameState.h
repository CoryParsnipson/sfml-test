#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../dependencies.h"

#include "Command/Command.h"
#include "Util/InputListener.h"

// forward declarations
class Game;
class GameStartMenuState;

class GameState : public InputListener {
public:
   static GameStartMenuState start_menu_state;

   virtual ~GameState() {}
   
   virtual void enter(Game& game) {}
   virtual void exit(Game& game) {}
   
   virtual GameState* update(Game& game) = 0;

   // command interface
   virtual void process(CloseCommand& c) = 0;
   virtual void process(KeyPressCommand& c) = 0;
   virtual void process(WindowResizeCommand& c) = 0;
   virtual void process(MouseMoveCommand& c) = 0;
   virtual void process(MouseButtonCommand& c) = 0;
   virtual void process(MouseWheelCommand& c) = 0;
};

#endif
