#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "Service.h"

class Observer;

class Subject {
public:
   using ObserverList = std::vector<Observer*>;

   Subject(std::string id = "Subject");

   void attach(Observer& o);
   void detach(Observer& o);

   void notify();

   friend std::ostream& operator<<(std::ostream& stream, const Subject& subject);

protected:
   std::string id_;
   ObserverList observers_;
};

class Observer {
public:
   using SubjectList = std::vector<Subject*>;

   Observer(std::string id = "Observer");
   virtual ~Observer();

   void add(Subject& subject);
   void remove(Subject& subject);

   virtual void notify(Subject& s) = 0;

   friend std::ostream& operator<<(std::ostream& stream, const Observer& observer);

protected:
   std::string id_;
   SubjectList subjects_;
};

#endif
