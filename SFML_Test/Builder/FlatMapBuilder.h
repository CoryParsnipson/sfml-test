#ifndef FLAT_MAP_BUILDER_H
#define FLAT_MAP_BUILDER_H

#include "MapBuilder.h"

class FlatMapBuilder : public MapBuilder {
public:
   FlatMapBuilder();
   virtual ~FlatMapBuilder();

   virtual void build();

protected:
   virtual void build_map();
   virtual void build_tile(Entity* tile);
   virtual void build_grid(Grid* grid);
};

#endif
