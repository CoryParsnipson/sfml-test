#include "VisualDebugSystem.h"

#include "Game.h"
#include "Scene.h"
#include "Entity.h"

#include "Collision.h"
#include "Rectangle.h"

#include "SetVisualDebugMessage.h"

VisualDebugSystem::VisualDebugSystem(const std::string& id, RenderSurface& surface, std::shared_ptr<Camera> camera)
: GraphicalSystem(id, surface, camera)
{
}

VisualDebugSystem::~VisualDebugSystem() {
}

void VisualDebugSystem::on_init(Game& game) {
   this->subscribe_to().one_of<Collision>();

   // handle messages to enable/disable visual debug
   this->install_message_handler<SetVisualDebugMessage>([&] (SetVisualDebugMessage& msg) {
      if (msg.enable) {
         this->enable();
      } else {
         this->disable();
      }
   });
}

void VisualDebugSystem::pre_update(Game& game) {
   this->camera()->draw(*this->surface()); // apply the camera
}

void VisualDebugSystem::on_update(Game& game, Entity& e) {
   Collision* c = e.get<Collision>();
   if (c && this->is_visible(e.handle())) {
      sf::RenderStates states;
      states.transform = this->global_transform(e);

      sf::RectangleShape r;
      r.setFillColor(sf::Color(124, 160, 210, 100));
      r.setOutlineColor(sf::Color(210, 124, 124, 100));
      r.setOutlineThickness(1.0);

      r.setSize(sf::Vector2f(c->volume().width, c->volume().height));
      r.setPosition(c->volume().left, c->volume().top);

      sf::RectangleShape center_dot;
      center_dot.setFillColor(sf::Color::White);
      center_dot.setSize(sf::Vector2f(1, 1));
      center_dot.setPosition(c->center());

      sf::Text t;
      t.setFont(this->scene().fonts().get("retro")->get_font());
      t.setFillColor(sf::Color::White);
      t.setString(c->id());
      t.setCharacterSize(10);
      t.setPosition(c->volume().left + 5, c->volume().top + 5);

      this->surface()->draw(r, states);
      this->surface()->draw(center_dot, states);
      this->surface()->draw(t, states);
   }
}
