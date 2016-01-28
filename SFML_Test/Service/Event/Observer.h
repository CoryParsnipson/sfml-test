#ifndef OBSERVER_H
#define OBSERVER_H

// forward declarations
class Event;

class Observer {
public:
   virtual void notify(Event* event) = 0;
};

#endif
