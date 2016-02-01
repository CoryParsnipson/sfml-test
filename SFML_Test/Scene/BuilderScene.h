//#ifndef BUILDER_SCENE_H
//#define BUILDER_SCENE_H
//
//#include "Scene.h"
//
//#include "MouseUtil.h"
//
//// forward declarations
//class Map;
//class MapBuilder;
//
//class BuilderScene
//: public Scene
//, public MouseControllable
//{
//public:
//   BuilderScene();
//   virtual ~BuilderScene();
//
//   virtual void enter(Game& game);
//   virtual void exit(Game& game);
//
//   // update interface
//   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);
//
//   // command interface
//   virtual void process(Game& game, CloseCommand& c);
//   virtual void process(Game& game, KeyPressCommand& c);
//   virtual void process(Game& game, WindowResizeCommand& c);
//   virtual void process(Game& game, MouseMoveCommand& c);
//   virtual void process(Game& game, MouseButtonCommand& c);
//   virtual void process(Game& game, MouseWheelCommand& c);
//
//   // mouse control interface
//   virtual void drag(MouseButtonCommand& c, sf::Vector2f delta);
//
//   virtual float get_scale();
//   virtual void set_scale(float factor);
//
//   virtual void click(MouseButtonCommand& c);
//
//protected:
//   Map* map;
//   MapBuilder* map_builder;
//
//   // mouse controllable state
//   sf::Vector2f* click_press_pos;
//   sf::Vector2f* click_release_pos;
//
//   // entities
//   Entity* e;  // temporary mouse placeholder
//   Entity* selected_tile;
//
//   sf::RectangleShape* origin_dot;
//   sf::RectangleShape* center_dot;
//   std::vector<sf::RectangleShape*> grid;
//
//   sf::Clock clock;
//   FontConfig fps_font;
//   int last_frame_time;
//   int frame_measurement_interval;
//   int frame_count;
//};
//
//#endif
