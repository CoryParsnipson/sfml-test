#ifndef SPACE_H
#define SPACE_H

#include <string>

#include "sfml.h"

#include "PooledComponent.h"

// ----------------------------------------------------------------------------
// Space
// 
// This is a spatial component. Entities need to have one of these components
// to be part of the current Scene's scene graph.
// ----------------------------------------------------------------------------
class Space : public PooledComponent<Space> {
public:
   explicit Space(const std::string& id = "Space Component");
   virtual ~Space();
   
   Space* parent();

   sf::Transform global_transform();
   sf::Transform local_transform(); // does not query parent pointers

   // TODO: fill this out, it's just a stub right now
private:
   Handle parent_;
   
   sf::RenderStates transform_;
   std::vector<Handle> children_;
};

#endif
