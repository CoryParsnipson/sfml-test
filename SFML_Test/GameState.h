#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../dependencies.h"

#include "Command/Command.h"

// forward declarations
class Game;
class GameState;
class GameStartMenuState;
class GameBuilderState;

class GameState {
public:
   static GameStartMenuState start_menu_state;
   static GameBuilderState builder_state;

   GameState() : is_running(true), next_state_(NULL) {}
   virtual ~GameState() {}
   
   virtual void enter(Game& game) {}
   virtual void exit(Game& game) {}
   
   virtual GameState* update(Game& game) = 0;

   // command interface
   virtual void process(Game& game, CloseCommand& c) = 0;
   virtual void process(Game& game, KeyPressCommand& c) = 0;
   virtual void process(Game& game, WindowResizeCommand& c) = 0;
   virtual void process(Game& game, MouseMoveCommand& c) = 0;
   virtual void process(Game& game, MouseButtonCommand& c) = 0;
   virtual void process(Game& game, MouseWheelCommand& c) = 0;

protected:
   bool is_running;
   GameState* next_state_;
};

#endif
