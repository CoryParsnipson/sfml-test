#ifndef GAME_STARTMENU_STATE_H
#define GAME_STARTMENU_STATE_H

#include "dependencies.h"

#include "GameState.h"

// forward declarations
class Game;

class GameStartMenuState : public GameState {
public:
   virtual void enter(Game& game);
   virtual void exit(Game& game);

   virtual GameState* update(Game& game);

   // command interface
   virtual void process(Game& game, CloseCommand& c);
   virtual void process(Game& game, KeyPressCommand& c);
   virtual void process(Game& game, WindowResizeCommand& c);
   virtual void process(Game& game, MouseMoveCommand& c);
   virtual void process(Game& game, MouseButtonCommand& c);
   virtual void process(Game& game, MouseWheelCommand& c);

protected:
   sf::Vector2f screen_middle;
   sf::Vector2f screen_size;
};

#endif
