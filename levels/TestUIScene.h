#ifndef TEST_UI_SCENE_H
#define TEST_UT_SCENE_H

#include "Scene.h"
#include "Settings.h"

// -----------------------------------------------------------------------------
// forward declarations
// -----------------------------------------------------------------------------
class Widget;
class GetSceneObjectCommand;

class TestUIScene
: public Scene
{
public:
   TestUIScene();
   ~TestUIScene();

   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

   // command interface
   virtual void process(Game& game, ResizeInputEvent& e);
   virtual void process(Game& game, KeyPressInputEvent& e);
   virtual void process(Game& game, MouseButtonInputEvent& e);

protected:
   Camera* ui_camera_;
   Entity* fps_display_;
   Widget* widget_;
   
   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;

   // helper methods
   void update_fps(Settings& settings);
};

#endif
