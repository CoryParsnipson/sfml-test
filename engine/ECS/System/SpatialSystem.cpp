#include <cassert>

#include "Space.h"
#include "Scene.h"
#include "SpatialSystem.h"
#include "AddToEntityMessage.h"
#include "RemoveFromEntityMessage.h"

SpatialSystem::SpatialSystem(const std::string& id /* = "SpatialSystem" */)
: System(id)
{
}

SpatialSystem::~SpatialSystem() {
}

void SpatialSystem::on_init(Game& game) {
   this->mailbox().handle<AddToEntityMessage>([&] (AddToEntityMessage& msg) {
      Entity* parent = this->scene().get_entity(msg.parent);
      Entity* child = this->scene().get_entity(msg.child);

      game.logger().msg(this->id(), Logger::INFO, "Handling AddToEntityMessage");

      if (parent == nullptr || child == nullptr) {
         return;
      }

      Entity* old_parent = this->scene().get_entity(child->get<Space>()->parent());
      if (old_parent) {
         old_parent->get<Space>()->remove(msg.child);
      }

      parent->get<Space>()->add(msg.child, msg.idx);
      child->get<Space>()->parent(msg.parent); // update child entity's parent handle
   });

   this->mailbox().handle<RemoveFromEntityMessage>([&] (RemoveFromEntityMessage& msg) {
      Entity* parent = this->scene().get_entity(msg.parent);
      Entity* child = this->scene().get_entity(msg.child);

      game.logger().msg(this->id(), Logger::INFO, "Handling RemoveFromEntityMessage");

      if (parent == nullptr || child == nullptr) {
         game.logger().msg(this->id(), Logger::WARNING, "RemoveFromEntityMessage has dead parent or child handle");
         return;
      }

      if (child->get<Space>()->parent() != msg.parent) {
         game.logger().msg(this->id(), Logger::WARNING,
            "RemoveFromEntityMessage given child that does not belong to parent. Parent -> " + \
            msg.parent.str() + \
            ", Child's parent -> " + \
            child->get<Space>()->parent().str()
         );
         return;
      }

      parent->get<Space>()->remove(msg.child);
      child->get<Space>()->parent(Handle()); // update child entity's parent handle
   });
}

void SpatialSystem::on_update(Game& game, Entity& e) {
   // SpatialSystem is odd in that nothing happens in its update
}
