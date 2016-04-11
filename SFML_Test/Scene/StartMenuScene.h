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

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
      
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
   bool show_debug_info_;

   // helper methods
   void toggle_debug_info();
};

#endif
