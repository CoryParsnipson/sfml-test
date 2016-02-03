#ifndef BUILDER_SCENE_H
#define BUILDER_SCENE_H

#include "Scene.h"
#include "MouseUtil.h"

class Map;
class MapBuilder;

class BuilderScene
: public Scene
, public MouseControllable
{
public:
   BuilderScene();
   virtual ~BuilderScene();

   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // draw interface
   virtual void draw(Graphics& graphics);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   // command interface
   virtual void process(Game& game, CloseCommand& c);
   virtual void process(Game& game, KeyPressCommand& c);
   virtual void process(Game& game, WindowResizeCommand& c);
   virtual void process(Game& game, MouseMoveCommand& c);
   virtual void process(Game& game, MouseButtonCommand& c);
   virtual void process(Game& game, MouseWheelCommand& c);

   // mouse control interface
   virtual void drag(MouseButtonCommand& c, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButtonCommand& c);

protected:
   // scene fixtures
   Map* map_;

   // entity pointers (no ownership)
   Entity* mouse_;
   Entity* tile_cursor_;
   
   Entity* center_dot_;

   // scene state
   sf::Vector2f reset_size_;
   sf::Vector2f reset_center_;

   // mouse controllable state
   sf::Vector2f last_mouse_pos_;

   sf::Vector2f* click_press_pos_;
   sf::Vector2f* click_release_pos_;

   std::vector<sf::RectangleShape*> grid;

   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;
};

#endif
