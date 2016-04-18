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
   SceneGraphNode(sf::RenderStates state = sf::RenderStates::Default);
   virtual ~SceneGraphNode();

   // scene graph interface
   void set_render_state(const sf::RenderStates& state_);
   const sf::RenderStates& get_render_state();

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) = 0;

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;

protected:
   sf::RenderStates state_;
};

#endif
