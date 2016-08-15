#ifndef SCENE_GRAPH_VISITOR_H
#define SCENE_GRAPH_VISITOR_H

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;
class Entity;
class Widget;
class Gamepad;

// ----------------------------------------------------------------------------
// SceneGraphVisitor interface
//
// This interface should be extended by any class that needs to traverse
// the scene graph and do non-trivial processing of the scene graph nodes.
// This is an implementation of the visitor pattern on the scene graph
// hierarchy.
// ----------------------------------------------------------------------------
class SceneGraphVisitor {
public:
   virtual void visit(Camera*) = 0;
   virtual void visit(Entity*) = 0;
   virtual void visit(Widget*) = 0;
   virtual void visit(Gamepad*) = 0;
};

#endif
