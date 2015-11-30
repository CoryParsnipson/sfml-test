#ifndef GAME_BUILDER_STATE_H
#define GAME_BUILDER_STATE_H

#include "dependencies.h"

#include "Entities/Map.h"
#include "Entities/Texture.h"
#include "Entities/Viewport.h"

#include "GameState.h"

// forward declarations
class Game;

class GameBuilderState : public GameState {
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
   std::map<std::string, Viewport*> viewports;

   Texture* t1;
   Texture* t2;

   Map* map;
};

#endif