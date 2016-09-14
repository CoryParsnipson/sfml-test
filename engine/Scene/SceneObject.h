#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "sfml.h"
#include "Draw.h"
#include "Update.h"
#include "Composite.h"
#include "SceneGraphVisitor.h"

class SceneObject
: public Draw
, public Update
, public Composite<SceneObject, true>
{
public:
   SceneObject(bool visible = true);
   virtual ~SceneObject() = 0;

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
   virtual void on_mouse_in();
   virtual void on_mouse_out();

   virtual void on_click();
   virtual void on_release();

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor) = 0;

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

private:
   bool visible_; // visibility of this node affects children as well

protected:
   sf::Transform transform_;

   // interface hooks for children 
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
