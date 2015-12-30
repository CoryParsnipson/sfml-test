#ifndef FLAT_MAP_BUILDER_H
#define FLAT_MAP_BUILDER_H

#include "MapBuilder.h"
#include "FlatMap.h"

class FlatMapBuilder : public MapBuilder {
public:
   FlatMapBuilder(TextureManager& tm);
   virtual ~FlatMapBuilder();

   virtual void build();

protected:
   virtual void build_map();
   virtual void build_tile(int x, int y, int texture);
};

#endif
