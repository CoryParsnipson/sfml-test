#ifndef TEST_UI_SCENE_H
#define TEST_UT_SCENE_H

#include "Scene.h"

// -----------------------------------------------------------------------------
// forward declarations
// -----------------------------------------------------------------------------
class Widget;
class GetWidgetCommand;

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
   virtual void process(Game& game, MouseButtonInputEvent& e);
   virtual void process(Game& game, ResizeInputEvent& e);

protected:
   Camera* hud_camera_;
   Entity* fps_display_;
   Widget* widget_;
   
   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;

   // helper methods
   void update_fps();
};

#endif
