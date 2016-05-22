#include "Observer.h"

std::ostream& operator<<(std::ostream& stream, const Subject& subject) {
   stream << "[Subject: " << subject.id_ << "]";
   return stream;
}

std::ostream& operator<<(std::ostream& stream, const Observer& observer) {
   stream << "[Observer: " << observer.id_ << "]";
   return stream;
}

Subject::Subject(std::string id /* = "Subject" */)
: id_(std::move(id))
{
}

void Subject::attach(Observer& o) {
   ObserverList::const_iterator it;
   for (it = this->observers_.begin(); it != this->observers_.end(); ++it) {
      if ((*it) == &o) {
         Service::get_logger().msg("Subject", Logger::WARNING, "Ignoring request to attach observer that is already attached.");
         return;
      }
   }

   o.add(*this);                   // add this subject to observer's subject list
   this->observers_.push_back(&o); // add this observer to subject's observer list
}

void Subject::detach(Observer& o) {
   ObserverList::const_iterator it = std::find(this->observers_.begin(), this->observers_.end(), &o);
   if (it != this->observers_.end()) {
      (*it)->remove(*this);
      this->observers_.erase(it);
   }
}

void Subject::notify() {
   ObserverList::const_iterator it;
   for (it = this->observers_.begin(); it != this->observers_.end(); ++it) {
      (*it)->notify(*this);
   }
}

Observer::Observer(std::string id /* = "Observer" */)
: id_(std::move(id))
{
}

Observer::~Observer() {
   SubjectList::const_iterator it = this->subjects_.begin();
   while (it != this->subjects_.end()) {
      // need to use while loop because detach modifies this list
      (*it)->detach(*this);
   }
}

void Observer::add(Subject& subject) {
   this->subjects_.push_back(&subject);
}

void Observer::remove(Subject& subject) {
   SubjectList::const_iterator it = std::find(this->subjects_.begin(), this->subjects_.end(), &subject);
   if (it != this->subjects_.end()) {
      this->subjects_.erase(it);
   }
}
