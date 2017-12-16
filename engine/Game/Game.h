#ifndef GAME_H
#define GAME_H

#include <map>
#include <stack>

#include "sfml.h"

#include "InputManager.h"
#include "Settings.h"
#include "Player.h"
#include "Logger.h"
#include "InputListener.h"
#include "Canvas.h"

class Scene;

class Game
: public InputListener
{
public:
   using Players = std::map<Player::PlayerId, Player*>;

   // services
   InputManager& input_manager();
   static Logger& logger();

	Game();
	virtual ~Game();

   // profiles
   Player& add_player(Player::PlayerId id);
   Player& get_player(Player::PlayerId id);

   // settings
   Settings settings;

   // game flow controls
   void start(); // run main loop
   void reset();

   // scene api
   void load_scene(Scene* scene);     // load new scene on top of current scene (and switch to it)
   void unload_scene();               // unload current scene and resume previous scene
   Scene* switch_scene(Scene* scene); // unload current scene and load new scene
   Scene* current_scene();

   // input event processing
   virtual void process(CloseInputEvent& e);
   virtual void process(ResizeInputEvent& e);
   virtual void process(KeyPressInputEvent& e);
   virtual void process(KeyReleaseInputEvent& e);
   virtual void process(MouseMoveInputEvent& e);
   virtual void process(MouseWheelInputEvent& e);
   virtual void process(MouseButtonInputEvent& e);

   bool poll_event(sf::Event& event);

   const Canvas& window() const;
   Canvas& window();

private:
   // services
   InputManager* input_manager_;
   static Logger* logger_;

   // player profiles
   Players players_;

	// game flow control
	void main_loop();

   Scene* next_scene_;  // if this is not null, it will be placed on the top of scenes_ in the next update
   Scene* prev_scene_;  // a state that is to be unloaded in the next update, should be popped off scenes_
   std::stack<Scene*> scenes_;

   // graphics
   Canvas window_;
};

#endif
