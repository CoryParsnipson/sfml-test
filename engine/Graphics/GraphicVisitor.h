#ifndef GRAPHIC_VISITOR_H
#define GRAPHIC_VISITOR_H

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class VertexList;
class Sprite;
class Text;
class CompositeGraphic;
class Rectangle;
class Circle;

// ----------------------------------------------------------------------------
// Graphic Visitor interface
// ----------------------------------------------------------------------------
class GraphicVisitor {
public:
   virtual void visit(VertexList* graphic) = 0;
   virtual void visit(Sprite* graphic) = 0;
   virtual void visit(Text* graphic) = 0;
   virtual void visit(CompositeGraphic* graphic) = 0;
   virtual void visit(Rectangle* graphic) = 0;
   virtual void visit(Circle* graphic) = 0;
};

#endif
