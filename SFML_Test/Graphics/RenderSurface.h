#ifndef RENDER_SURFACE_H
#define RENDER_SURFACE_H

#include "dependencies.h"
#include "Draw.h"

class Game;
class Scene;
class Entity;
class Camera;

class RenderSurface {
public:
   using LayerList = std::map<int, std::vector<std::tuple<Draw*, sf::RenderStates> > >;

   RenderSurface() {}
   virtual ~RenderSurface() {
      this->layers_.clear();
   }

   // set the current camera
   virtual void set_camera(Camera& camera) = 0;

   // draw interface
   virtual void draw(Draw& drawable, sf::RenderStates render_states = sf::RenderStates::Default) = 0;
   virtual void draw(sf::Drawable& drawable, sf::RenderStates render_states = sf::RenderStates::Default) = 0;

   // update interface
   virtual void update() = 0;
   virtual void clear(const sf::Color* color = nullptr) = 0;

protected:
   LayerList layers_;
};

#endif
