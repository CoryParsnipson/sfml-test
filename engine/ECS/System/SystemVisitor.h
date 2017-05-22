#ifndef SYSTEM_VISITOR_H
#define SYSTEM_VISITOR_H

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class InputSystem;

class SystemVisitor {
public:
   virtual void visit(InputSystem* system) = 0;
};

#endif
