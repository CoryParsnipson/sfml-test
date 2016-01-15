#ifndef NULL_EVENT_QUEUE_H
#define NULL_EVENT_QUEUE_H

#include "EventQueue.h"

class NullEventQueue : public EventQueue {
   virtual void push_event(Event* event) {};
   virtual void register_listener(Entity* observer) {};
};

#endif
