#ifndef GAME_BUILDER_STATE_H
#define GAME_BUILDER_STATE_H

#include "dependencies.h"

#include "GameState.h"

#include "Texture.h"
#include "Viewport.h"

#include "Entity.h"

// forward declarations
class Map;
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

   sf::Clock clock;
   
   Map* map;
   
   Entity* e;
   std::vector<Entity*> tiles;

   sf::RectangleShape* origin_dot;
   sf::RectangleShape* center_dot;
   std::vector<sf::RectangleShape*> grid;
};

#endif
