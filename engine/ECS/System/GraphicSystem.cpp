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
#include "ResizeCameraMessage.h"

GraphicSystem::GraphicSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera)
: System(id, new SpatialEntitySubscription(id + "EntitySubscription"))
, surface_(&surface)
, camera_(camera)
{
}

GraphicSystem::~GraphicSystem() {
}

std::shared_ptr<Camera> GraphicSystem::camera() const {
   return this->camera_;
}

void GraphicSystem::on_init(Game& game) {
   this->subscribe_to().one_of<Circle, Rectangle, Sprite, Text, VertexList>();
   this->subscribe_to().all_of<Space>();

   // handle resize messages
   this->mailbox().handle<ResizeCameraMessage>([this](ResizeCameraMessage& msg) {
      this->camera_->resize(sf::Vector2f(msg.width, msg.height));
   });
}

void GraphicSystem::on_update(Game& game, Entity& e) {
   Circle* circle = e.get<Circle>();
   Rectangle* rectangle = e.get<Rectangle>();
   Sprite* sprite = e.get<Sprite>();
   Text* text = e.get<Text>();
   VertexList* vertexlist = e.get<VertexList>();

   sf::RenderStates states;
   states.transform = this->global_transform(e);

   this->camera_->draw(*this->surface_); // apply the camera

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
