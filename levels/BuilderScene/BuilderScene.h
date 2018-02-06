#ifndef BUILDER_SCENE_H
#define BUILDER_SCENE_H

#include "Scene.h"
#include "Settings.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Channel;
class Texture;
class Map;

// ----------------------------------------------------------------------------
// BuilderScene
// ----------------------------------------------------------------------------
class BuilderScene
: public Scene
{
public:
   BuilderScene();
   virtual ~BuilderScene();

   virtual void init(Game& game);
   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // input event processing
   virtual void process(Game& game, CloseInputEvent& e);
};

#endif
