#include <algorithm>

#include "MapSystem.h"
#include "RenderSurface.h"
#include "Camera.h"

#include "ResizeCameraMessage.h"

#include "TileMap.h"

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
   std::vector<TileMap::Tile*> tiles = map->find(this->camera_->bounds());

   states.transform = this->global_transform(e);
   
   this->camera_->draw(*this->surface_); // apply camera

   // draw map tiles
   std::for_each(tiles.begin(), tiles.end(),
      [&] (TileMap::Tile* tile) {
         tile->draw(*this->surface_, states);
      }
   );
}
