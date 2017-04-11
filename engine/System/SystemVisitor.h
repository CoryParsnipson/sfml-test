#ifndef SYSTEM_VISITOR_H
#define SYSTEM_VISITOR_H

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class InputSystem;
class GraphicsSystem;

class SystemVisitor {
public:
   virtual void visit(InputSystem* system) = 0;
   virtual void visit(GraphicsSystem* system) = 0;
};

#endif
