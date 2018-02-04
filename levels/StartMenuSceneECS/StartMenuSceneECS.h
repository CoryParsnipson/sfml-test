#ifndef START_MENU_SCENE_ECS_H
#define START_MENU_SCENE_ECS_H

#include "Scene.h"

class StartMenuSceneECS
: public Scene
{
public:
   StartMenuSceneECS();
   ~StartMenuSceneECS();

   virtual void init(Game& game);
   virtual void enter(Game& game);
   virtual void exit(Game& game);
};

#endif
