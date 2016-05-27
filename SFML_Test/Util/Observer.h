#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <algorithm>
#include <iostream>

// ----------------------------------------------------------------------------
// useful things
// ----------------------------------------------------------------------------
template<typename C>
using iterator = typename C::iterator;

template<typename C>
using const_iterator = typename C::const_iterator;

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
template <class T> class Observer;
template <class T> class Subject;

// ----------------------------------------------------------------------------
// Subject base class
// 
// This is the subject base class that contains a list of observers which it
// can notify. It is recommended to inherit from this and implement one's own
// child class that calls notify with the proper parameterized message type
// but not necessary.
// ----------------------------------------------------------------------------
template <class T>
class Subject {
public:
   using ObserverList = std::vector<Observer<T>*>;

   Subject(std::string id = "Subject");
   virtual ~Subject();

   bool attach(Observer<T>& observer);
   bool detach(Observer<T>& observer);

   void notify(T& message);

   operator std::string() const {
      return this->to_string();
   }

private:
   template <class E>
   friend std::ostream& operator<<(std::ostream& stream, const Subject<E>& subject);

   std::string to_string() const {
      return "[Subject: " + this->id_ + "]";
   }

protected:
   std::string id_;
   ObserverList observers_;
};

// ----------------------------------------------------------------------------
// Object abstract base
// 
// This is the observer base class that can be attached to subjects. This is
// an abstract base class that requires the notify function to be implemented.
// Observers with a specific type T can only be attached and detached from
// Subjects with the same type T.
// ----------------------------------------------------------------------------
template <class T>
class Observer {
public:
   using SubjectList = std::vector<Subject<T>*>;

   // ----------------------------------------------------------------------------
   // public interface
   // ----------------------------------------------------------------------------
   Observer(std::string id = "Observer");
   virtual ~Observer();

   virtual void notify(T& message) = 0;

   operator std::string() const {
      return this->to_string();
   }

private:
   friend class Subject<T>;

   template <class E>
   friend std::ostream& operator<<(std::ostream& stream, const Observer<E>& observer);

   // for Subject's use
   void add(Subject<T>& subject);
   void remove(Subject<T>& subject);

   std::string to_string() const {
      return "[Observer: " + this->id_ + "]";
   }

protected:
   std::string id_;
   SubjectList subjects_;
};

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
template <class T>
std::ostream& operator<<(std::ostream& stream, const Subject<T>& subject) {
   stream << subject.to_string();
   return stream;
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const Observer<T>& observer) {
   stream << observer.to_string();
   return stream;
}

// ----------------------------------------------------------------------------
// Subject constructor implementations
// ----------------------------------------------------------------------------
template <class T>
Subject<T>::Subject(std::string id /* = "Subject" */)
: id_(std::move(id))
{
}

template <class T>
Subject<T>::~Subject() {
   const_iterator<ObserverList> it;
   for (it = this->observers_.begin(); it != this->observers_.end(); ++it) {
      (*it)->remove(*this);
   }
}

// ----------------------------------------------------------------------------
// Subject method implementations
// ----------------------------------------------------------------------------
template <class T>
bool Subject<T>::attach(Observer<T>& observer) {
   const_iterator<ObserverList> it;
   for (it = this->observers_.begin(); it != this->observers_.end(); ++it) {
      if ((*it) == &observer) {
         //Service::get_logger().msg("Subject", Logger::WARNING, "(id = " + this->id_ + ") Ignoring request to attach observer that is already attached.");
         return false;
      }
   }

   observer.add(*this);                   // add this subject to observer's subject list
   this->observers_.push_back(&observer); // add this observer to subject's observer list
   return true;
}

template <class T>
bool Subject<T>::detach(Observer<T>& observer) {
   const_iterator<ObserverList> it = std::find(this->observers_.begin(), this->observers_.end(), &observer);
   if (it != this->observers_.end()) {
      (*it)->remove(*this);
      this->observers_.erase(it);
   } else {
      //Service::get_logger().msg("Subject", Logger::WARNING, "Failed to detach " + observer);
      return false;
   }

   return true;
}

template <class T>
void Subject<T>::notify(T& message) {
   const_iterator<ObserverList> it;
   for (it = this->observers_.begin(); it != this->observers_.end(); ++it) {
      (*it)->notify(message);
   }
}

// ----------------------------------------------------------------------------
// Observer constructor implementations
// ----------------------------------------------------------------------------
template <class T>
Observer<T>::Observer(std::string id /* = "Observer" */)
: id_(std::move(id))
{
}

template <class T>
Observer<T>::~Observer() {
   const_iterator<SubjectList> it = this->subjects_.begin();

   // need to use while loop because detach modifies this list
   while (it != this->subjects_.end()) {
      if (!(*it)->detach(*this)) {
         it = this->subjects_.erase(it);
      }
   }
}

// ----------------------------------------------------------------------------
// Observer method implementations
// ----------------------------------------------------------------------------
template <class T>
void Observer<T>::add(Subject<T>& subject) {
   this->subjects_.push_back(&subject);
}

template <class T>
void Observer<T>::remove(Subject<T>& subject) {
   const_iterator<SubjectList> it = std::find(this->subjects_.begin(), this->subjects_.end(), &subject);
   if (it != this->subjects_.end()) {
      this->subjects_.erase(it);
   }
}

#endif
