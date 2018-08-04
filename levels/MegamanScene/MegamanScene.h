#ifndef MEGAMAN_SCENE_H
#define MEGAMAN_SCENE_H

#include "Scene.h"

class MegamanScene
: public Scene
{
public:
   MegamanScene();
   ~MegamanScene();

   virtual void init(Game& game);
   virtual void enter(Game& game);
   virtual void exit(Game& game);
};

#endif
