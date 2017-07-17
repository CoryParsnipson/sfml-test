#ifndef SPACE_H
#define SPACE_H

#include <string>

#include "sfml.h"

#include "Component.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// Space
// 
// This is a spatial component. Entities need to have one of these components
// to be part of the current Scene's scene graph.
// ----------------------------------------------------------------------------
class Space : public Component {
public:
   explicit Space(const std::string& id = "Space Component");
   virtual ~Space();
   
   // TODO: fill this out, it's just a stub right now
private:
   sf::RenderStates transform_;
   std::vector<Handle> children_;
};

#endif
