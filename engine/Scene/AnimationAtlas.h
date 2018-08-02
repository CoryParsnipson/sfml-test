#ifndef ANIMATION_ATLAS_H
#define ANIMATION_ATLAS_H

#include <string>
#include <SFML/Graphics.hpp>

#include "Atlas.h"
#include "Animation.h"
#include "Serializable.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Scene;

// ----------------------------------------------------------------------------
// AnimationAtlas
//
// This is a map that contains Animations.
// ----------------------------------------------------------------------------
class AnimationAtlas
: public Atlas<std::string, Animation>
, public Serializable
{
   AnimationAtlas();
   virtual ~AnimationAtlas();

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);
};

#endif
