#ifndef BUILDER_SCENE_ECS_H
#define BUILDER_SCENE_ECS_H

#include "Scene.h"
#include "Settings.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Channel;
class Texture;
class Map;

// ----------------------------------------------------------------------------
// BuilderSceneECS
// ----------------------------------------------------------------------------
class BuilderSceneECS
: public Scene
{
public:
   BuilderSceneECS();
   virtual ~BuilderSceneECS();

   virtual void init(Game& game);
   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // input event processing
   virtual void process(Game& game, CloseInputEvent& e);

protected:
   // scene fixtures
   Channel* map_file_;
};

#endif
