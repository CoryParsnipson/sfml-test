#include <cassert>
#include <functional>

#include "System.h"
#include "Game.h"
#include "Scene.h"

#include "AddToEntityMessage.h"
#include "RemoveFromEntityMessage.h"
#include "ComponentAddedMessage.h"
#include "ComponentRemovedMessage.h"

System::System(const std::string& id /* = "System" */, EntitySubscription* sub /* = new BaseEntitySubscription() */)
: id_(id)
, enabled_(true)
, scene_(nullptr)
, mailbox_(id + "Mailbox")
, subscription_(sub)
{
}

System::~System() {
   delete this->subscription_;
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

   if (!this->scene_) {
      game.logger().msg(this->id(), Logger::WARNING, "Trying to initialize system when Game has no loaded scenes. Skipping.");
      return;
   }

   this->pre_init(game);
   this->on_init(game);

   // filter in subscription list should be initialized in pre_init() or on_init()
   this->subscription_->init(*this); 

   // add some reasonable defaults to certain message types
   this->mailbox().handle<ComponentAddedMessage>([this](ComponentAddedMessage& msg) {
      this->subscription().clear();
      this->subscription().init(*this);
   });

   this->mailbox().handle<ComponentRemovedMessage>([this](ComponentRemovedMessage& msg) {
      this->subscription().clear();
      this->subscription().init(*this);
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
   this->subscription_->for_each(*this, handle_on_update);

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

void System::subscription(EntitySubscription* subscription) {
   if (!subscription) {
      Game::logger().msg(this->id(), Logger::WARNING, "Setting EntitySubscription to null pointer.");
      return;
   }
   
   delete this->subscription_;
   this->subscription_ = subscription;
}

EntityFilter& System::subscribe_to() {
   return this->subscription_->filter();
}

sf::Transform System::local_transform(Entity& e) {
   Space* space = e.get<Space>();
   if (space != nullptr) {
      return space->states().transform;
   }

   return sf::Transform();
}

sf::Transform System::global_transform(Entity& e) {
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

void System::send_message_helper(std::shared_ptr<Message> message) {
   assert(this->scene_ != nullptr);
   this->scene_->handle_message(message);
}
