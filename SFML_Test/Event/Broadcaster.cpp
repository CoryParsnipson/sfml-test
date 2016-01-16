#include "Broadcaster.h"

#include "dependencies.h"

#include "Event.h"
#include "Observer.h"

void Broadcaster::broadcast(Event* event) {
   observer_t::const_iterator it;
   for (it = this->observers_.begin(); it != this->observers_.end(); ++it) {
      (*it)->notify(event);
   }
}

void Broadcaster::register_observer(Observer* observer) {
   // make sure there are no duplicates
   observer_t::const_iterator it;
   for (it = this->observers_.begin(); it != this->observers_.end(); ++it) {
      if (observer == *it) {
         Service::get_logger().msg("Broadcaster", Logger::WARNING, "Ignoring registering observer already in broadcast list.");
         return;
      }
   }

   this->observers_.push_back(observer);
}

void Broadcaster::unregister_observer(Observer* observer) {
   observer_t::const_iterator it;
   for (it = this->observers_.begin(); it != this->observers_.end(); ++it) {
      if (observer == *it) {
         this->observers_.erase(it);
         return;
      }
   }
}
