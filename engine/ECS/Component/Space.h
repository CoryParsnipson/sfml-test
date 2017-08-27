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

   Handle parent() const;
   void parent(Handle parent);

   Handle add(unsigned int idx) const;
   void add(Handle child, int idx = -1);

   void remove(Handle child);
   void remove(unsigned int idx);

   Handle get(unsigned int idx) const;

   unsigned int num_children() const;

   sf::RenderStates& states();
   void states(sf::RenderStates states);

private:
   sf::RenderStates transform_;

   Handle parent_;
   std::vector<Handle> children_;
};

#endif
