#ifndef GAME_BUILDER_STATE_H
#define GAME_BUILDER_STATE_H

#include "dependencies.h"

#include "Entities/Map.h"

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
   sf::Vector2f screen_middle;
   sf::Vector2f screen_size;
   
   sf::Vector2f origin;

   Map* map;

   sf::Image* tile1_nomask;
   sf::Image* tile2_nomask;

   sf::Texture* tile1;
   sf::Texture* tile2;

   Mouse* m;
};

#endif
