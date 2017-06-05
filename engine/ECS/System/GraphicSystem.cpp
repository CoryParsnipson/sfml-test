#include "GraphicSystem.h"
#include "RenderSurface.h"
#include "Game.h"
#include "Camera.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "Text.h"
#include "VertexList.h"

GraphicSystem::GraphicSystem(const std::string& id, RenderSurface& surface) 
: System(id)
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
   this->filter().one_of<Circle, Rectangle, Sprite, Text, VertexList>();
   //this->filter_.all_of<Space/*, Position (???)*/>();
}

void GraphicSystem::on_update(Game& game, Entity& e) {
   Circle* circle = e.get<Circle>();
   Rectangle* rectangle = e.get<Rectangle>();
   Sprite* sprite = e.get<Sprite>();
   Text* text = e.get<Text>();
   VertexList* vertexlist = e.get<VertexList>();

   for (std::vector<Camera*>::const_iterator camera = this->cameras_.begin(); camera != this->cameras_.end(); ++camera) {
      // apply this camera
      (*camera)->draw(*this->surface_);

//      if (circle != nullptr) {
//         circle->draw(*this->surface_);
//      }

      if (rectangle != nullptr) {
         rectangle->draw(*this->surface_);
      }

      if (sprite != nullptr) {
         sprite->draw(game.window());
      }

      if (text != nullptr) {
         text->draw(*this->surface_);
      }

//      if (vertexlist != nullptr) {
//         vertexlist->draw(*this->surface_);
//      }
   }
}
