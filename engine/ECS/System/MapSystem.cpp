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
: System(id)
, surface_(&surface)
, camera_(camera)
{
}

MapSystem::~MapSystem() {
}

void MapSystem::on_init(Game& game) {
   this->subscribe_to().all_of<TileMap>();

   // handle resize messages
   this->mailbox().handle<ResizeCameraMessage>([this](ResizeCameraMessage& msg) {
      this->camera_->resize(sf::Vector2f(msg.width, msg.height));
   });
}

void MapSystem::on_update(Game& game, Entity& e) {
   sf::RenderStates states;
   TileMap* map = e.get<TileMap>();
   std::vector<TileMap::TileType*> tiles = map->find(this->camera_->bounds());

   states.transform = this->global_transform(e);
   
   this->camera_->draw(*this->surface_); // apply camera

   // draw map tiles
   std::for_each(tiles.begin(), tiles.end(),
      [&] (TileMap::TileType* tile) {
         if (tile->type() == typeid(Circle)) {
            boost::get<Circle>(*tile).draw(*this->surface_, states);
         } else if (tile->type() == typeid(Rectangle)) {
            boost::get<Rectangle>(*tile).draw(*this->surface_, states);
         } else if (tile->type() == typeid(Sprite)) {
            boost::get<Sprite>(*tile).draw(*this->surface_, states);
         } else if (tile->type() == typeid(Text)) {
            boost::get<Text>(*tile).draw(*this->surface_, states);
         } else if (tile->type() == typeid(VertexList)) {
            boost::get<VertexList>(*tile).draw(*this->surface_, states);
         }
      }
   );
}
