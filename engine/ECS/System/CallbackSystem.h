#ifndef CALLBACK_SYSTEM_H
#define CALLBACK_SYSTEM_H

#include "System.h"
#include "Callback.h"

// ----------------------------------------------------------------------------
// CallbackSystem
//
// This system looks for all objects with a Callback component and calculates
// if there are any callback events registered in that component that should
// be fired off.
// ----------------------------------------------------------------------------
class CallbackSystem : public System {
public:
   CallbackSystem(const std::string& id = "CallbackSystem");
   virtual ~CallbackSystem();

private:
   bool camera_was_resized_;
   std::map<CallbackKey, bool> target_hit_;

   virtual void on_init(Game& game);

   virtual void pre_update(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
