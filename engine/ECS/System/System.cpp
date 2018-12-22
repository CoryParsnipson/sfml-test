#include <cassert>
#include <functional>

#include "System.h"
#include "Game.h"
#include "Scene.h"

#include "ComponentAddedMessage.h"
#include "ComponentRemovedMessage.h"

System::System(const std::string& id /* = "System" */, EntitySubscription* sub /* = nullptr */)
: Messageable(id)
, id_(id)
, enabled_(true)
, scene_(nullptr)
, mailbox_(id + "Mailbox")
, subscription_(sub ? sub : new BaseEntitySubscription(this))
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
   this->subscription_->init();

   this->post_init(game);
}

void System::update(Game& game) {
   if (!this->is_enabled()) {
      return;
   }

   assert(this->scene_ != nullptr);

   this->pre_update(game);
   this->handle_queued_messages();

   this->subscription_->handle_queued_messages();
   this->subscription_->update();

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

void System::break_out_of_update() {
   this->subscription_->break_out_of_update();
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

Handle System::root() const {
   return this->root_;
}

void System::root(Handle root) {
   this->root_ = root;
}

EntityFilter& System::subscribe_to() {
   return this->subscription_->filter();
}

bool System::is_visible(Handle entity) {
   bool visible = true;
   Entity* e = this->scene().get_entity(entity);
   if (!e) {
      return false;
   }

   SceneNode* space = e->space();

   while (space && visible) {
      visible = visible && space->visible();
      space = space->parent();
   }

   return visible;
}

sf::Transform System::global_transform(Entity& e) {
   return this->scene().global_transform(e);
}

void System::add_to_scene_node(Handle parent, Handle child, int idx /* = -1 */) {
   this->scene().add_to_scene_node(parent, child, idx);
}

void System::add_to_scene_node(Entity* parent, Handle child, int idx /* = -1 */) {
   this->scene().add_to_scene_node(parent, child, idx);
}

void System::add_to_scene_node(Handle parent, Entity* child, int idx /* = -1 */) {
   this->scene().add_to_scene_node(parent, child, idx);
}

void System::add_to_scene_node(Entity* parent, Entity* child, int idx /* = -1 */) {
   this->scene().add_to_scene_node(parent, child, idx);
}

void System::add_to_scene_node(SceneNode* parent, Entity* child, int idx /* = -1 */) {
   this->scene().add_to_scene_node(parent, child, idx);
}

void System::add_to_scene_node(SceneNode* parent, SceneNode* child, int idx /* = -1 */) {
   this->scene().add_to_scene_node(parent, child, idx);
}

void System::send_message_helper(MessagePtr message) {
   assert(this->scene_);
   this->scene_->receive_message(message);
}

void System::post_receive_message(MessagePtr message) {
   // forward message to entity subscription
   this->subscription().receive_message(message);
}
