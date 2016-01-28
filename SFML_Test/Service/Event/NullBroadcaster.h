#ifndef NULL_BROADCASTER_H
#define NULL_BROADCASTER_H

#include "Broadcaster.h"

class NullBroadcaster : public Broadcaster {
public:
   virtual void broadcast(Event* event) {}

   virtual void register_observer(Observer* observer) {}
   virtual void unregister_observer(Observer* observer) {}
};

#endif
