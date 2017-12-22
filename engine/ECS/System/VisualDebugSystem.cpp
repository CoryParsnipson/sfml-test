#include "VisualDebugSystem.h"
#include "Game.h"
#include "Entity.h"
#include "Collision.h"
#include "Rectangle.h"

VisualDebugSystem::VisualDebugSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera)
: GraphicalSystem(id, surface, camera)
{
}

VisualDebugSystem::~VisualDebugSystem() {
}

void VisualDebugSystem::on_init(Game& game) {
   this->subscribe_to().one_of<Collision>();
}

void VisualDebugSystem::on_update(Game& game, Entity& e) {
   Collision* c = e.get<Collision>();
   if (c) {
      sf::RectangleShape r;
      r.setFillColor(sf::Color(124, 160, 210, 200));
      r.setOutlineColor(sf::Color(210, 124, 124, 200));
      r.setOutlineThickness(1.0);

      r.setSize(sf::Vector2f(c->volume().width, c->volume().height));
      r.setPosition(c->volume().left, c->volume().top);

      this->surface()->draw(r);
   }
}
