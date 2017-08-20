#include <cassert>
#include <algorithm>
#include <functional>

#include "System.h"
#include "Game.h"
#include "Scene.h"

#include "EntityCreatedMessage.h"
#include "AddToEntityMessage.h"
#include "RemoveFromEntityMessage.h"

System::System(const std::string& id /* = "System" */, EntitySubscription* sub /* = new BaseEntitySubscription() */)
: id_(id)
, enabled_(true)
, scene_(nullptr)
, mailbox_(id + "Mailbox")
, subscription_(sub)
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
   this->subscription_->scene(*game.current_scene());

   if (!this->scene_) {
      game.logger().msg(this->id(), Logger::WARNING, "Trying to initialize system when Game has no loaded scenes. Skipping.");
      return;
   }

   this->pre_init(game);
   this->on_init(game);

   // filter in subscription list should be initialized in pre_init() or on_init()
   this->subscription_->init(); 

   // add some reasonable defaults to certain message types
   this->mailbox().handle<EntityCreatedMessage>([this](EntityCreatedMessage& msg) {
      this->subscription().add(msg.entity);
   });

   this->post_init(game);
}

void System::update(Game& game) {
   if (!this->is_enabled()) {
      return;
   }

   assert(this->scene_ != nullptr);

   this->pre_update(game);

   // handle any queued up messages
   this->mailbox_.process_queue();

   // perform system update on all subscribed entities
   std::function<void(Handle)> handle_on_update = [&](Handle h) {
      Entity* e = this->scene_->get_entity(h);
      if (e) {
         this->on_update(game, *e);
      }
   };
   this->subscription_->for_each(handle_on_update);

   this->post_update(game);
}

Scene& System::scene() {
   assert(this->scene_ != nullptr);
   return *this->scene_;
}

Mailbox& System::mailbox() {
   return this->mailbox_;
}

EntitySubscription& System::subscription() {
   return *this->subscription_;
}

EntityFilter& System::subscribe_to() {
   return this->subscription_->filter();
}

void System::send_message_helper(std::shared_ptr<Message> message) {
   assert(this->scene_ != nullptr);
   this->scene_->handle_message(message);
}
