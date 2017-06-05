#include <cassert>
#include <algorithm>

#include "System.h"
#include "Game.h"
#include "Scene.h"

System::System(const std::string& id /* = "System" */)
: id_(id)
, enabled_(true)
, filter_(id + "EntityFilter")
{
}

System::~System() {
}

void System::id(const std::string& id) {
   this->id_ = id;
}

const std::string& System::id() const {
   return this->id_;
}

EntityFilter& System::filter() {
   return this->filter_;
}

bool System::filter(Entity& e) const {
   return this->filter_.filter(e);
}

void System::enable() {
   this->enabled_ = true;
}

void System::disable() {
   this->enabled_ = false;
}

bool System::is_enabled() const {
   return this->enabled_;
}

void System::init(Game& game) {
   this->pre_init(game);
   this->on_init(game);

   // filter should be initialized by now, generate subscribed entities list
   Scene* scene = game.current_scene();
   if (scene) {
      std::vector<Handle> entities_from_scene = scene->entities();

      std::for_each(entities_from_scene.begin(), entities_from_scene.end(),
         [&](Handle& h) {
            Entity* e = scene->get_entity(h);
            assert(e != nullptr);

            if (this->filter(*e)) {
               this->entities_.push_back(h);
            }
         }
      );
   }

   this->post_init(game);
}

void System::update(Game& game) {
   if (!this->is_enabled()) {
      return;
   }

   this->pre_update(game);

   // call on_update for each subscribed entity
   Scene* scene = game.current_scene();
   if (scene) {
      std::for_each(this->entities_.begin(), this->entities_.end(), 
         [&](Handle& h) {
            Entity* e = scene->get_entity(h);
            if (e) {
               this->on_update(game, *e);
            }
         }
      );
   }

   this->post_update(game);
}

void System::message() {
   if (!this->is_enabled()) {
      return;
   }

   // TODO: expand this stub
}
