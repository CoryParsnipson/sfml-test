#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "dependencies.h"

template <class T>
class Composite
{
public:
   using CompositeList = std::vector<T*>;

   Composite() {}
   virtual ~Composite() {}

   // composite interface
   virtual void add(T* child) {
      if (!child) {
         Service::get_logger().msg("Composite", Logger::WARNING, "Adding null child to composite.");
         return;
      }
      this->children_.push_back(child);
   }

   virtual T* get(int idx) {
      if (idx < 0 || idx >= this->get_num_children()) {
         return nullptr;
      }

      return this->children_[idx];
   };

   virtual void remove(T* child) {
      Iterator<CompositeList> it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         if (*it == child) {
            delete *it;
            this->children_.erase(it);
            return;
         }
      }
   }

   virtual void remove(int idx) {
      if (idx < 0 || idx >= this->get_num_children()) {
         return;
      }
      this->children_.erase(this->children_.begin() + idx);
   }

   int get_num_children() { return this->children_.size(); };

protected:
   CompositeList children_;
};

template<class T>
class Leaf : public Composite<T> {
public:
   // composite interface
   virtual void add(T* child) {}
   virtual T* get(int idx) { return nullptr; }
   virtual void remove(T* child) {}
   virtual void remove(int idx) {}
};
#endif
