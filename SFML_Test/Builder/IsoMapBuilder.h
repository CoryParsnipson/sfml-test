#ifndef ISO_MAP_BUILDER_H
#define ISO_MAP_BUILDER_H

#include "MapBuilder.h"
#include "IsoMap.h"

class IsoMapBuilder : public MapBuilder {
public:
   IsoMapBuilder(TextureManager& tm);
   virtual ~IsoMapBuilder();

   virtual void create_map();
   virtual Entity* load_tile();
};

#endif
