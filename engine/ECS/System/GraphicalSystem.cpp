#include <algorithm>

#include <boost/variant.hpp>

#include "GraphicalSystem.h"
#include "RenderSurface.h"
#include "Game.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "Text.h"
#include "VertexList.h"
#include "TileMap.h"
#include "Space.h"
#include "PreorderEntitySubscription.h"
#include "Scene.h"
#include "ResizeCameraMessage.h"

GraphicalSystem::GraphicalSystem(const std::string& id, RenderSurface& surface, CameraPtr camera)
: System(id, new PreorderEntitySubscription(this, id + "EntitySubscription"))
, surface_(&surface)
, camera_(camera)
{
   // handle resize messages
   this->install_message_handler<ResizeCameraMessage>([this](ResizeCameraMessage& msg) {
      this->camera_->size(sf::Vector2f(msg.width, msg.height));
   });
}

GraphicalSystem::~GraphicalSystem() {
}

CameraPtr GraphicalSystem::camera() const {
   return this->camera_;
}

RenderSurface* GraphicalSystem::surface() const {
   return this->surface_;
}

void GraphicalSystem::on_init(Game& game) {
   this->subscribe_to().one_of<Circle, Rectangle, Sprite, Text, VertexList, TileMap>();
   this->subscribe_to().all_of<Space>();
}

void GraphicalSystem::pre_update(Game& game) {
   this->camera_->draw(*this->surface_); // apply the camera
}

void GraphicalSystem::on_update(Game& game, Entity& e) {
   if (!this->is_visible(e.handle())) {
      // skip the hidden stuff
      return;
   }

   Circle* circle = e.get<Circle>();
   Rectangle* rectangle = e.get<Rectangle>();
   Sprite* sprite = e.get<Sprite>();
   Text* text = e.get<Text>();
   VertexList* vertexlist = e.get<VertexList>();
   TileMap* tilemap = e.get<TileMap>();

   sf::RenderStates states;
   states.transform = this->global_transform(e);

   if (circle != nullptr) {
      circle->draw(*this->surface_, states);
   }

   if (rectangle != nullptr) {
      rectangle->draw(*this->surface_, states);
   }

   if (sprite != nullptr) {
      sprite->draw(*this->surface_, states);
   }

   if (text != nullptr) {
      text->draw(*this->surface_, states);
   }

   if (vertexlist != nullptr) {
      vertexlist->draw(*this->surface_, states);
   }

   if (tilemap != nullptr) {
      std::vector<TileMap::TileType*> tiles = tilemap->find(states.transform.getInverse().transformRect(this->camera()->bounds()));

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
}
