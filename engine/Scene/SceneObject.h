#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "sfml.h"
#include "Draw.h"
#include "Update.h"
#include "Composite.h"
#include "Serializable.h"
#include "SceneGraphVisitor.h"

// ----------------------------------------------------------------------------
// forward declarations 
// ----------------------------------------------------------------------------
class Command;

// ----------------------------------------------------------------------------
// SceneObject
//
// This class is the basis for every object that can be present as an entity
// in a scene.
// ----------------------------------------------------------------------------
class SceneObject
: public Draw
, public Update
, public Composite<SceneObject, true>
, public Serializable
{
public:
   SceneObject(bool visible = true);
   virtual ~SceneObject() = 0;

   operator std::string() const;
   virtual std::string to_string() const;

   // scene graph interface
   SceneObject* layer(int idx) const;
   int num_layers() const;

   bool visible() const;
   void visible(bool visible);

   sf::Transform global_transform() const;
   sf::Transform local_transform() const; // does not query parent pointers

   virtual sf::Vector2f get_world_coordinate(const sf::Vector2f& point); // affected by camera transform
   virtual sf::Vector2f get_screen_coordinate(const sf::Vector2f& point); // relative to viewport

   virtual bool intersects(const sf::Vector2i& other) = 0;
   virtual bool intersects(const sf::Vector2f& other) = 0;
   virtual bool intersects(const sf::FloatRect& other) = 0;
   virtual bool intersects(const SceneObject& other) = 0;

   // mouse interaction interface
   void propagate_event(bool propagate_event);
   bool propagate_event() const;

   bool clickable() const;
   void clickable(bool clickable);

   virtual void on_mouse_in();
   virtual void on_mouse_in(Command* cmd);

   virtual void on_mouse_out();
   virtual void on_mouse_out(Command* cmd);

   virtual void on_mouse_move();
   virtual void on_mouse_move(Command* cmd);

   virtual void on_click();
   virtual void on_click(Command* cmd);

   virtual void on_left_click();
   virtual void on_left_click(Command* cmd);

   virtual void on_right_click();
   virtual void on_right_click(Command* cmd);

   virtual void on_release();
   virtual void on_release(Command* cmd);

   virtual void on_left_release();
   virtual void on_left_release(Command* cmd);

   virtual void on_right_release();
   virtual void on_right_release(Command* cmd);

   virtual void on_mouse_wheel();
   virtual void on_mouse_wheel(Command* cmd);

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor) = 0;

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

private:
   bool visible_; // visibility of this node affects children as well
   bool propagate_event_; // false to stop this node from passing mouse events to parent node
   friend std::ostream& operator<<(std::ostream& stream, const SceneObject& object);

   bool clickable_;  // if true, intersect with gamepad cursor checks

   Command* on_mouse_in_;
   Command* on_mouse_out_;
   Command* on_mouse_move_;
   Command* on_click_;
   Command* on_left_click_;
   Command* on_right_click_;
   Command* on_release_;
   Command* on_left_release_;
   Command* on_right_release_;
   Command* on_mouse_wheel_;

protected:
   sf::Transform transform_;

   // interface hooks for children 
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
