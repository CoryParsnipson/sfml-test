#include "GraphicSystem.h"
#include "RenderSurface.h"
#include "Game.h"
#include "Camera.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "Text.h"
#include "VertexList.h"
#include "Space.h"
#include "SpatialEntitySubscription.h"
#include "Scene.h"

GraphicSystem::GraphicSystem(const std::string& id, RenderSurface& surface) 
: System(id, new SpatialEntitySubscription(id + "EntitySubscription"))
, surface_(&surface)
{
   // provide a default camera
   this->cameras_.push_back(new Camera("Main Camera"));
}

GraphicSystem::~GraphicSystem() {
   for (std::vector<Camera*>::const_iterator it = this->cameras_.begin(); it != this->cameras_.end(); ++it) {
      delete *it;
   }
   this->cameras_.clear();
}

Camera* GraphicSystem::camera(unsigned int idx /* = 0 */) const {
   if (idx >= this->cameras_.size()) {
      return nullptr;
   }
   return this->cameras_[idx];
}

void GraphicSystem::on_init(Game& game) {
   this->subscribe_to().one_of<Circle, Rectangle, Sprite, Text, VertexList>();
   this->subscribe_to().all_of<Space>();
}

void GraphicSystem::on_update(Game& game, Entity& e) {
   Circle* circle = e.get<Circle>();
   Rectangle* rectangle = e.get<Rectangle>();
   Sprite* sprite = e.get<Sprite>();
   Text* text = e.get<Text>();
   VertexList* vertexlist = e.get<VertexList>();

   sf::RenderStates states;
   states.transform = this->global_transform(e);

   for (std::vector<Camera*>::const_iterator camera = this->cameras_.begin(); camera != this->cameras_.end(); ++camera) {
      // apply this camera
      (*camera)->draw(*this->surface_);

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
   }
}

sf::Transform GraphicSystem::local_transform(Entity& e) {
   Space* space = e.get<Space>();
   if (space != nullptr) {
      return space->states().transform;
   }

   return sf::Transform();
}

sf::Transform GraphicSystem::global_transform(Entity& e) {
   sf::Transform g_transform = sf::Transform();
   Space* space = e.get<Space>();
   Entity* entity = &e;

   while (space != nullptr) {
      g_transform *= space->states().transform;

      entity = this->scene().get_entity(space->parent());
      if (entity != nullptr) {
         space = entity->get<Space>();
      } else {
         break;
      }
   }

   return g_transform;
}
