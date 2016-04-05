#ifndef GAME_H
#define GAME_H

#include "dependencies.h"
#include "FullLogger.h"
#include "InputListener.h"
#include "RenderWindow.h"
#include "TextureManager.h"

class CloseCommand;
class KeyPressCommand;
class WindowResizeCommand;
class MouseMoveCommand;
class MouseButtonCommand;
class MouseWheelCommand;

class Scene;

class Game
: public InputListener
{
public:
	Game();
	virtual ~Game();

   // game flow controls
   void start(); // run main loop
   void reset();

   // scene api
   void load_scene(Scene* scene);     // load new scene on top of current scene (and switch to it)
   void unload_scene();               // unload current scene and resume previous scene
   Scene* switch_scene(Scene* scene); // unload current scene and load new scene

   // game graphics
   RenderWindow& get_window();

   // command interface
   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);
   virtual void process(MouseButtonCommand& c);
   virtual void process(MouseWheelCommand& c);

protected:
	// game flow control
	void main_loop();

   Scene* next_scene_;  // if this is not null, it will be placed on the top of scenes_ in the next update
   Scene* prev_scene_;  // a state that is to be unloaded in the next update, should be popped off scenes_
   std::stack<Scene*> scenes_;

   // services
   FullLogger full_logger_;
   InputController input_;

   // graphics
   RenderWindow window_;
};

#endif
