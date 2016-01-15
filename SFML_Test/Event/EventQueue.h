#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"

class EventQueue {
public:
   typedef std::vector<Event*> queue_t;

protected:
   queue_t events_;
};

#endif
