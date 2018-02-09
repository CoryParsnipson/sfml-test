#ifndef START_MENU_SCENE_H
#define START_MENU_SCENE_H

#include "Scene.h"

class StartMenuScene
: public Scene
{
public:
   StartMenuScene();
   ~StartMenuScene();

   virtual void init(Game& game);
   virtual void enter(Game& game);
   virtual void exit(Game& game);

   virtual void process(Game& game, MouseEnteredInputEvent& e);
   virtual void process(Game& game, MouseLeftInputEvent& e);

private:
   Handle mouse_cursor_;
};

#endif
