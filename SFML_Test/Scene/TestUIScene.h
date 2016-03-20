#ifndef TEST_UI_SCENE_H
#define TEST_UT_SCENE_H

#include "Scene.h"

class TestUIScene
: public Scene
{
public:
   TestUIScene();
   ~TestUIScene();

   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   Entity* mouse_;
};

#endif
