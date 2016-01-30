#ifndef START_MENU_SCENE_H
#define START_MENU_SCENE_H

#include "Scene.h"
#include "FontConfig.h"

class StartMenuScene
: public Scene
{
public:
   StartMenuScene();
   ~StartMenuScene();

   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   // command interface
   virtual void process(Game& game, CloseCommand& c);
   virtual void process(Game& game, KeyPressCommand& c);
   virtual void process(Game& game, WindowResizeCommand& c);
   virtual void process(Game& game, MouseMoveCommand& c);
   virtual void process(Game& game, MouseButtonCommand& c);
   virtual void process(Game& game, MouseWheelCommand& c);

protected:
   FontConfig font_header_;
   FontConfig font_subtitle_;
};

#endif
