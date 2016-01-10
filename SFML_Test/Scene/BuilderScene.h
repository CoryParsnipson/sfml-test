#ifndef BUILDER_SCENE_H
#define BUILDER_SCENE_H

#include "Scene.h"

// forward declarations
class Map;
class Entity;

class BuilderScene : public Scene {
public:
   virtual void enter(Game& game);
   virtual void exit(Game& game);

   virtual void update(Game& game);

   // command interface
   virtual void process(Game& game, CloseCommand& c);
   virtual void process(Game& game, KeyPressCommand& c);
   virtual void process(Game& game, WindowResizeCommand& c);
   virtual void process(Game& game, MouseMoveCommand& c);
   virtual void process(Game& game, MouseButtonCommand& c);
   virtual void process(Game& game, MouseWheelCommand& c);

protected:
   sf::Clock clock;

   Map* map;

   // entities
   Entity* e;  // temporary mouse placeholder

   sf::RectangleShape* origin_dot;
   sf::RectangleShape* center_dot;
   std::vector<sf::RectangleShape*> grid;
};

#endif
