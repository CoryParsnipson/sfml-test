#ifndef START_MENU_SCENE_H
#define START_MENU_SCENE_H

#include "Scene.h"

class StartMenuScene
: public Scene
{
public:
   StartMenuScene();
   ~StartMenuScene();

   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // input event processing
   virtual void process(Game& game, ResizeInputEvent& e);
};

#endif
