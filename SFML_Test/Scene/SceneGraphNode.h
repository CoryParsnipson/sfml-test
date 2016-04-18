#ifndef SCENE_GRAPH_NODE_H
#define SCENE_GRAPH_NODE_H

#include "dependencies.h"
#include "Composite.h"
#include "Draw.h"
#include "Update.h"

class SceneGraphNode
: public Composite<SceneGraphNode>
, public Draw
, public Update
{
public:
   SceneGraphNode(sf::RenderStates state = sf::RenderStates::Default, bool visible = true);
   virtual ~SceneGraphNode();

   // scene graph interface
   void set_render_state(const sf::RenderStates& state_);
   sf::RenderStates get_render_state();

   bool visible() const;
   void visible(bool visible);

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   sf::RenderStates state_;
   bool visible_; // visibility for this node AND IT'S CHILDREN

   // interface hooks for CHILDREN
   virtual void pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {}
   virtual void post_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {}

   virtual void pre_update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) {}
   virtual void post_update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) {}
};

#endif
