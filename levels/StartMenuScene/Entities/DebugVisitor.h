#ifndef DEBUG_VISITOR
#define DEBUG_VISITOR

#include "sfml.h"
#include "SceneGraphVisitor.h"

class DebugVisitor : public SceneGraphVisitor {
public:
   DebugVisitor(bool show_debug_info = false, sf::Font* font = nullptr);

   void show_debug(bool show);
   bool show_debug();

   virtual void visit(Camera*);
   virtual void visit(Entity*);
   virtual void visit(Map*);
   virtual void visit(Grid*);
   virtual void visit(Widget*);
   virtual void visit(Gamepad*);

private:
   bool show_debug_info_;
   sf::Font* font_;
};

#endif
