#include <algorithm>

#include <boost/variant.hpp>

#include "MapSystem.h"
#include "RenderSurface.h"
#include "Camera.h"
#include "ResizeCameraMessage.h"
#include "TileMap.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "Text.h"
#include "VertexList.h"

MapSystem::MapSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera)
: GraphicalSystem(id, surface, camera)
{
   this->subscription(new BaseEntitySubscription());
}

MapSystem::~MapSystem() {
}

void MapSystem::on_init(Game& game) {
   this->subscribe_to().all_of<TileMap>();
}

void MapSystem::on_update(Game& game, Entity& e) {
   sf::RenderStates states;
   TileMap* map = e.get<TileMap>();
   std::vector<TileMap::TileType*> tiles = map->find(this->camera()->bounds());

   states.transform = this->global_transform(e);
   
   this->camera()->draw(*this->surface()); // apply camera

   // draw map tiles
   std::for_each(tiles.begin(), tiles.end(),
      [&] (TileMap::TileType* tile) {
         if (tile->type() == typeid(Circle)) {
            boost::get<Circle>(*tile).draw(*this->surface(), states);
         } else if (tile->type() == typeid(Rectangle)) {
            boost::get<Rectangle>(*tile).draw(*this->surface(), states);
         } else if (tile->type() == typeid(Sprite)) {
            boost::get<Sprite>(*tile).draw(*this->surface(), states);
         } else if (tile->type() == typeid(Text)) {
            boost::get<Text>(*tile).draw(*this->surface(), states);
         } else if (tile->type() == typeid(VertexList)) {
            boost::get<VertexList>(*tile).draw(*this->surface(), states);
         }
      }
   );
}
