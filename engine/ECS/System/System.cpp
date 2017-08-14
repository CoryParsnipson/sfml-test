#include <cassert>
#include <algorithm>
#include <functional>

#include "System.h"
#include "Game.h"
#include "Scene.h"

#include "EntityCreatedMessage.h"
#include "AddToEntityMessage.h"
#include "RemoveFromEntityMessage.h"

System::System(const std::string& id /* = "System" */)
: id_(id)
, enabled_(true)
, scene_(nullptr)
, mailbox_(id + "Mailbox")
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
   assert(this->scene_ == nullptr);
   this->scene_ = game.current_scene();

   if (!this->scene_) {
      game.logger().msg(this->id(), Logger::WARNING, "Trying to initialize system when Game has no loaded scenes. Skipping.");
      return;
   }

   this->pre_init(game);
   this->on_init(game);

   // filter should be initialized by now, generate subscribed entities list
   std::vector<Handle> entities_from_scene = this->scene_->entities();
   std::for_each(entities_from_scene.begin(), entities_from_scene.end(),
      std::bind(&System::filter_entity, this, std::placeholders::_1)
   );

   // add some reasonable defaults to certain message types
   this->mailbox().handle<EntityCreatedMessage>([this](EntityCreatedMessage& msg) {
      this->filter_entity(msg.entity);
   });

   this->post_init(game);
}

void System::update(Game& game) {
   if (!this->is_enabled()) {
      return;
   }

   this->pre_update(game);

   // handle any queued up messages
   this->mailbox_.process_queue();

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

Scene& System::scene() {
   assert(this->scene_ != nullptr);
   return *this->scene_;
}

Mailbox& System::mailbox() {
   return this->mailbox_;
}

const std::vector<Handle>& System::subscribed_entities() {
   return this->entities_;
}

int System::on_add_entity(Handle entity) {
   return -1;
}

void System::send_message_helper(std::shared_ptr<Message> message) {
   assert(this->scene_ != nullptr);
   this->scene_->handle_message(message);
}

void System::add_entity(Handle entity) {
   int idx = this->on_add_entity(entity);

   if (idx < 0 || static_cast<unsigned int>(idx) >= this->entities_.size()) {
      this->entities_.push_back(entity);
   } else {
      this->entities_.insert(this->entities_.begin() + idx, entity);
   }
}

void System::filter_entity(Handle entity) {
   assert(this->scene_ != nullptr);

   Entity* e = this->scene_->get_entity(entity);
   if (e != nullptr && this->filter(*e)) {
      this->add_entity(entity);
   }
}
