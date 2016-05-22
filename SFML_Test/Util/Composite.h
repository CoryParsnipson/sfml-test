#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "dependencies.h"
#include <iterator>

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
class Composite
{
public:
   using CompositeList = std::vector<T*>;

   // iterators
   class prefix_iterator;
   // class infix_iterator;
   // class postfix_iterator;
   // class breadth_iterator;
   // class depth_iterator;

   Composite();
   virtual ~Composite() = 0;

   // client interface
   void add(T* child);
   T* get(int idx);
   void remove(T* child);
   void remove(int idx);

   int get_num_children();

   // iterator interface
   virtual prefix_iterator begin() { return prefix_iterator(static_cast<T*>(this)); }
   virtual prefix_iterator end() { return prefix_iterator(); }

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
: public std::iterator<std::bidirectional_iterator_tag, T>
{
public:
   prefix_iterator() {}
   prefix_iterator(T* itr) {
      this->idx_.push_back(0);
      this->nodes_.push_back(itr);
   }

   virtual Composite<T>::prefix_iterator& operator++() {
      if (this->nodes_.empty()) {
         return *this;
      }

      while (this->idx_.back() == this->nodes_.back()->get_num_children()) {
         // reached a leaf node or finished traversing all children of current node...
         this->idx_.pop_back();
         this->nodes_.pop_back();

         if (this->nodes_.empty()) {
            break;
         }

         ++this->idx_.back();
         if (this->idx_.back() < this->nodes_.back()->get_num_children()) {
            this->nodes_.push_back(this->nodes_.back()->get(this->idx_.back()));
            this->idx_.push_back(0);
            break;
         }
      }

      if (this->nodes_.empty()) {
         return *this;
      }

      if (this->idx_.back() < this->nodes_.back()->get_num_children()) {
         // descend to to next child
         this->nodes_.push_back(this->nodes_.back()->get(this->idx_.back()));
         this->idx_.push_back(0);
      }

      return *this;
   }

   virtual Composite<T>::prefix_iterator operator++(int) {
      Composite<T>::prefix_iterator tmp(*this);

      ++(*this);
      return tmp;
   }

   virtual bool operator!=(const Composite<T>::prefix_iterator& itr) {
      bool this_empty = this->nodes_.empty();
      bool other_empty = itr.nodes_.empty();

      return (this_empty != other_empty) || (!this_empty && !other_empty && (this->nodes_.back() != itr.nodes_.back()));
   }

   virtual bool operator==(const Composite<T>::prefix_iterator& itr) {
      bool this_empty = this->nodes_.empty();
      bool other_empty = itr.nodes_.empty();

      return (this_empty && other_empty) || (this->nodes_.back() == itr.nodes_.back());
   }

   virtual T* operator*() {
      if (this->nodes_.empty()) {
         // should throw an exception instead?
         return nullptr;
      }

      return this->nodes_.back();
   }

   virtual T* operator->() {
      if (this->nodes_.empty()) {
         // should throw an exception instead?
         return nullptr;
      }

      return this->nodes_.back();
   }

protected:
   std::vector<int> idx_;
   std::vector<T*> nodes_;
};

#endif
