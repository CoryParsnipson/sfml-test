#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "dependencies.h"
#include "Iterator.h"

// ----------------------------------------------------------------------------
// Composite abstract base
//
// This is an archetype specification for classes where instances may have
// any number of children of the same inheritance family.
//
// Concrete implementations should only override methods under the "hooks"
// section of the class definition.
// ----------------------------------------------------------------------------
template <class T>
class Composite {
public:
   using CompositeList = std::vector<T*>;

   // iterators
   class prefix_iterator;
   class infix_iterator;
   class postfix_iterator;
   class breadth_iterator;
   class depth_iterator;

   Composite();
   virtual ~Composite();

   // client interface
   void add(T* child);
   T* get(int idx);
   void remove(T* child);
   void remove(int idx);

   int get_num_children();

protected:
   CompositeList children_;

   // hooks for child interface
   virtual void add_pre(T* child) {}
   virtual void add_post(T* child) {}

   virtual void get_pre(int idx) {}
   virtual void get_post(int idx) {}

   virtual void remove_pre(T* child) {}
   virtual void remove_post(T* child) {}

   virtual void remove_pre(int idx) {}
   virtual void remove_post(int idx) {}
};

// ----------------------------------------------------------------------------
// Composite constructor implementations
// ----------------------------------------------------------------------------
template <class T>
Composite<T>::Composite() {
}

template <class T>
Composite<T>::~Composite() {
   this->children_.clear();
}

// ----------------------------------------------------------------------------
// Composite method implementations
// ----------------------------------------------------------------------------
template <class T>
void Composite<T>::add(T* child) {
   if (!child) {
      return;
   }

   this->add_pre(child);
   this->children_.push_back(child);
   this->add_post(child);
}

template <class T>
T* Composite<T>::get(int idx) {
   if (idx < 0 || idx >= this->get_num_children()) {
      return nullptr;
   }

   this->get_pre(idx);
   return this->children_[idx];
   this->get_post(idx);
}

template <class T>
void Composite<T>::remove(T* child) {
   this->remove_pre(child);

   iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if (*it == child) {
         delete *it;
         this->children_.erase(it);
         return;
      }
   }

   this->remove_post(child);
}

template <class T>
void Composite<T>::remove(int idx) {
   if (idx < 0 || idx >= this->get_num_children()) {
      return;
   }

   this->remove_pre(idx);
   this->children_.erase(this->children_.begin() + idx);
   this->remove_post(idx);
}

template <class T>
int Composite<T>::get_num_children() {
   return this->children_.size();
}

// ----------------------------------------------------------------------------
// Composite prefix iterator
// ----------------------------------------------------------------------------
template <class T>
class Composite<T>::prefix_iterator
: public Iterator<Composite<T> >
{
public:
   prefix_iterator(Composite<T>& iterable);
   virtual ~prefix_iterator();

   virtual void begin();
   virtual void end();

   Iterator<Composite<T> >& operator++(); // prefix
   Iterator<Composite<T> >& operator++(int); // postfix

protected:
   Composite<T>* iterable_;

   T* begin_;
   T* end_;
};

template <class T>
Composite<T>::prefix_iterator::prefix_iterator(Composite<T>& iterable)
: Iterator<T>(iterable)
{
}

template <class T>
Composite<T>::prefix_iterator::~prefix_iterator() {
}

template <class T>
void Composite<T>::prefix_iterator::begin() {
}

template <class T>
void Composite<T>::prefix_iterator::end() {
}

#endif
