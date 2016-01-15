#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

// forward declarations
class Event;
class Entity;

class EventQueue {
public:
   typedef std::vector<Event*> queue_t;

   virtual void push_event(Event* event) = 0;
   virtual void register_listener(Entity* observer) = 0;

protected:
   queue_t events_;
};

#endif
