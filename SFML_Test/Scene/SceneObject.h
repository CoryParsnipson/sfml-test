#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "Draw.h"
#include "Update.h"
#include "Composite.h"

class SceneObject
: public Draw
, public Update
, Composite<SceneObject, true>
{
public:
   // scene graph interface
   bool visible() const;
   void visible(bool visible);

   const sf::Transform& transform() const;

   virtual bool intersects(const sf::Vector2i& other) = 0;
   virtual bool intersects(const sf::Vector2f& other) = 0;
   virtual bool intersects(const sf::FloatRect& other) = 0;
   virtual bool intersects(const SceneObject& other) = 0;

   // mouse interaction interface
   virtual void on_mouse_in() {};
   virtual void on_mouse_out() {};

   virtual void on_click() {};
   virtual void on_release() {};

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor) = 0;

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {
      this->draw_pre(surface, render_states);

      

      this->draw_post(surface, render_states);
   }

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr) {
      this->update_pre(game, scene);

      

      this->update_post(game, scene);
   }

protected:
   bool visible_; // visibility of this node affects children as well
   sf::Transform transform_;

   // interface hooks for children 
   virtual void draw_pre(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {}
   virtual void draw_post(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {}

   virtual void update_pre(Game& game, Scene* scene = nullptr) {}
   virtual void update_post(Game& game, Scene* scene = nullptr) {}
};

#endif
