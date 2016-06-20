#ifndef SCENE_GRAPH_VISITOR_H
#define SCENE_GRAPH_VISITOR_H

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;
class Draw;
class Entity;
class Widget;

// ----------------------------------------------------------------------------
// SceneGraphVisitor interface
//
// This interface should be extended by any class that needs to traverse
// the scene graph and do non-trivial processing of the scene graph nodes.
// This is an implementation of the visitor pattern on the scene graph
// hierarchy. The other half of this interface is in the SceneGraphNode class.
// ----------------------------------------------------------------------------
class SceneGraphVisitor {
public:
   virtual void visit(Camera*) = 0;
   virtual void visit(Draw*) = 0;
   virtual void visit(Entity*) = 0;
   virtual void visit(Widget*) = 0;
};

#endif
