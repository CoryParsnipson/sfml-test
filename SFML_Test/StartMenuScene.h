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
   virtual void process(Game& game, CloseInputEvent& e);
   virtual void process(Game& game, ResizeInputEvent& e);
   virtual void process(Game& game, KeyPressInputEvent& e);
   virtual void process(Game& game, MouseMoveInputEvent& e);
   virtual void process(Game& game, MouseWheelInputEvent& e);
   virtual void process(Game& game, MouseButtonInputEvent& e);

protected:
   bool show_debug_info_;

   // helper methods
   void toggle_debug_info();
};

#endif
