#ifndef ORTHOGRAHPIC_MAP_BUILDER_H
#define ORTHOGRAHPIC_MAP_BUILDER_H

#include "MapBuilder.h"

class OrthographicMapBuilder : public MapBuilder {
public:
   OrthographicMapBuilder();
   virtual ~OrthographicMapBuilder();

   virtual void build();

protected:
   virtual void build_map();
   virtual void build_tile(Entity* tile);
   virtual void build_grid(Grid* grid);
};

#endif
