#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <stack>
#include <tuple>

#include "sfml.h"
#include "SceneGraphVisitor.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;
class SceneObject;
class RenderSurface;

// ----------------------------------------------------------------------------
// SceneRenderer
//
// This class traverses the scene graph and draws all the nodes.
// ----------------------------------------------------------------------------
class SceneRenderer : public SceneGraphVisitor {
public:
   // scene graph visitor interface
   virtual void visit(Camera* camera);
   virtual void visit(Entity* entity);
   virtual void visit(Map* map);
   virtual void visit(Grid* grid);
   virtual void visit(Widget* widget);
   virtual void visit(Gamepad* gamepad);

   // renderer interface
   void render(SceneObject& scene_graph, RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

private:
   RenderSurface* surface_;

   std::stack<Camera*> cameras_;
   std::stack<sf::RenderStates> render_states_;
   std::stack<std::tuple<SceneObject*, bool> > nodes_;

   void push(SceneObject* node, sf::RenderStates render_states);
   void pop();

   void draw(sf::Transform local_transform = sf::Transform::Identity);
};

#endif
