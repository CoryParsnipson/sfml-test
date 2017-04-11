#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "System.h"
#include "Camera.h"
#include "SceneGraphVisitor.h"
#include "Canvas.h"

class GraphicsSystem
: public System
, public SceneGraphVisitor
{
public:
   using CameraList = std::vector<Camera*>;

   GraphicsSystem(const std::string& id = "GraphicsSystem");
   virtual ~GraphicsSystem();

   virtual void update(Game& game);

   virtual void accept(SystemVisitor& visitor);

private:
   CameraList cameras_;

   Canvas* window_;

   virtual void visit(Camera*);
   virtual void visit(Entity*);
   virtual void visit(Map*);
   virtual void visit(Grid*);
   virtual void visit(Widget*);
   virtual void visit(Gamepad*);
};

#endif
