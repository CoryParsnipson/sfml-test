#ifndef BUILDER_SCENE_H
#define BUILDER_SCENE_H

#include "Scene.h"
#include "MouseUtil.h"

class Texture;

class Map;
class MapBuilder;
class Serializer;

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
   sf::VertexArray backdrop_;
   std::string map_filename_;
   Serializer* serializer_;

   // entity pointers (no ownership)
   Entity* mouse_;
   Entity* center_dot_;
   Entity* selection_rectangle_;
   Entity* tile_cursor_;
   Entity* fps_display_;

   // mouse controllable state
   sf::Vector2f click_press_pos_;

   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;
   bool show_debug_info_;

   // helper methods
   void update_fps();

   void toggle_debug_info();

   void register_selection_rect();
   void deregister_selection_rect();
   void update_selection_rect(sf::Vector2f& origin_click, sf::Vector2f& mouse_pos);

   void update_tile_cursor(sf::Vector2f& one, sf::Vector2f& two);
   void remove_tile_cursor();

   void set_tiles(Texture& tile_texture);
   void remove_tiles();
};

#endif
