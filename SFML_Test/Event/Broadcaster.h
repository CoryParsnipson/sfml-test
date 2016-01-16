#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <vector>

// forward declarations
class Event;
class Observer;

class Broadcaster {
public:
   typedef std::vector<Observer*> observer_t;

   virtual void broadcast(Event* event);

   virtual void register_observer(Observer* observer);
   virtual void unregister_observer(Observer* observer);

protected:
   observer_t observers_;
};

#endif
