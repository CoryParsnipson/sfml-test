#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <iostream>
#include <vector>
#include <iterator>

// -----------------------------------------------------------------------------
// forward declarations
// -----------------------------------------------------------------------------
class TraitsPrefix;
class TraitsPostfix;
class TraitsBreadth;

template <class T, class Traits> class CompositeIterator;

// -----------------------------------------------------------------------------
// aliases
// -----------------------------------------------------------------------------
using IndexList = std::vector<int>;

template <class T>
using NodeList = std::vector<T*>;

// -----------------------------------------------------------------------------
// Composite iterator trait definitions
// -----------------------------------------------------------------------------
template <class T, class Traits>
class CompositeIteratorTraits;

template <class T>
class CompositeIteratorTraits<T, TraitsPrefix> {
public:
   // any initialization steps necessary
   static void init(NodeList<T>& nodes, IndexList& indices) {
      // empty
   }

   // retrive current node in iteration
   static T* get(NodeList<T>& nodes, IndexList& indices) {
      return nodes.back();
   }

   // increment the current node in the iteration
   static void next(NodeList<T>& nodes, IndexList& indices) {
      // if the current node we are on still has unvisited children, descend to next child
      if (!nodes.empty() && indices.back() < nodes.back()->get_num_children()) {
         nodes.push_back(nodes.back()->get(indices.back()));
         indices.push_back(0);
         return;
      }

      // we have reached a leaf node or finished visiting all children
      while (!nodes.empty() && indices.back() == nodes.back()->get_num_children()) {
         // pop current node because we have already visited this node
         // (because we are doing prefix iteration)
         indices.pop_back();
         nodes.pop_back();

         // do the if statement from above here
         // NOTE: indices should be the same length as nodes
         if (!nodes.empty() && ++indices.back() < nodes.back()->get_num_children()) {
            nodes.push_back(nodes.back()->get(indices.back()));
            indices.push_back(0);
            break;
         }
      }
   }
};

template <class T>
class CompositeIteratorTraits<T, TraitsPostfix> {
public:
   // any initialization steps necessary
   static void init(NodeList<T>& nodes, IndexList& indices) {
      if (nodes.empty()) {
         return;
      }
   
      // move to the first leaf node
      while (indices.back() < nodes.back()->get_num_children()) {
         nodes.push_back(nodes.back()->get(indices.back()));
         indices.push_back(0);
      }
   }

   // retrive current node in iteration
   static T* get(NodeList<T>& nodes, IndexList& indices) {
      return nodes.back();
   }

   // increment the current node in the iteration
   static void next(NodeList<T>& nodes, IndexList& indices) {
      if (nodes.empty()) {
         return;
      }

      if (indices.back() < nodes.back()->get_num_children()) {
         // we still have children to traverse
         nodes.push_back(nodes.back()->get(indices.back()));
         indices.push_back(0);

         next(nodes, indices);
      } else {
         // we have finished traversing all children for this node (or we hit a leaf node), pop it
         nodes.pop_back();
         indices.pop_back();
         
         if (nodes.empty()) {
            return;
         }

         // increment the number of children traversed on parent node
         ++indices.back();

         // and continue traverse until we hit the next leaf node
         while (indices.back() < nodes.back()->get_num_children()) {
            nodes.push_back(nodes.back()->get(indices.back()));
            indices.push_back(0);
         }
      }
   }
};

template <class T>
class CompositeIteratorTraits<T, TraitsBreadth> {
public:
   // any initialization steps necessary
   static void init(NodeList<T>& nodes, IndexList& indices) {
      // empty
   }

   // retrive current node in iteration
   static T* get(NodeList<T>& nodes, IndexList& indices) {
      return nodes.front();
   }

   // increment the current node in the iteration (breadth iteration does not use indexlist)
   static void next(NodeList<T>& nodes, IndexList& indices) {
      if (nodes.empty()) {
         return;
      }

      // push all children onto list
      for (int i = 0; i < nodes.front()->get_num_children(); ++i) {
         nodes.push_back(nodes.front()->get(i));
      }

      // pop the front of the nodelist (it was the old current node)
      nodes.erase(nodes.begin());
   }
};

// -----------------------------------------------------------------------------
// Composite abstract base
//
// This is an archetype specification for classes where instances may have
// any number of children of the same inheritance family.
//
// Concrete implementations should only override methods under the "hooks"
// section of the class definition.
// -----------------------------------------------------------------------------
template <class T, typename Traits = TraitsPrefix>
class Composite
{
public:
   using iterator         = typename std::vector<T*>::iterator;
   using const_iterator   = typename std::vector<T*>::const_iterator;

   using prefix_iterator  = CompositeIterator<T, TraitsPrefix>;
   using postfix_iterator = CompositeIterator<T, TraitsPostfix>;
   using breadth_iterator = CompositeIterator<T, TraitsBreadth>;

   Composite();
   virtual ~Composite() = 0;

   // client interface
   void add(T* child);
   T* get(int idx);
   void remove(T* child);
   void remove(int idx);

   int get_num_children();

   // iterator interface
   virtual CompositeIterator<T, Traits> begin();
   virtual CompositeIterator<T, Traits> end();

protected:
   std::vector<T*> children_;

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

// -----------------------------------------------------------------------------
// Composite iterator class
//
// This class implements a basic bi-direction standard library iterator for
// composite classes. Essentially, it allows for general n-tree traversals. This
// class uses the "traits" template meta-programming technique to enable
// different iterator types (prefix traversal, postfix traversal, etc) to be
// used with one begin() and end() method signature like the stl containers.
// -----------------------------------------------------------------------------
template <class T, typename Traits = TraitsPrefix>
class CompositeIterator
: public std::iterator<std::bidirectional_iterator_tag, T>
{
public:
   // NOTE: need to add a line for each type of trait
   friend class CompositeIterator<T, TraitsPrefix>;
   friend class CompositeIterator<T, TraitsPostfix>;
   friend class CompositeIterator<T, TraitsBreadth>;

   CompositeIterator() {
      CompositeIteratorTraits<T, Traits>::init(this->nodes_, this->indices_);
   }

   CompositeIterator(T* itr) {
      this->indices_.push_back(0);
      this->nodes_.push_back(itr);
      
      CompositeIteratorTraits<T, Traits>::init(this->nodes_, this->indices_);
   }

   CompositeIterator(const CompositeIterator<T, Traits>& other)
   : indices_(other.indices_)
   , nodes_(other.nodes_)
   {
      CompositeIteratorTraits<T, Traits>::init(this->nodes_, this->indices_);
   }

   // assignment operator overload
   // NOTE: needed to convert between CompositeIterators with different traits
   CompositeIterator operator=(CompositeIterator<T, TraitsPrefix> other) {
      // check for self assignment and skip if so
      if ((void*)this != (void*)&other) {
         this->nodes_ = other.nodes_;
         this->indices_ = other.indices_;
      }

      CompositeIteratorTraits<T, Traits>::init(this->nodes_, this->indices_);
      return *this;
   }

   // ++operator overload
   virtual CompositeIterator& operator++() {
      CompositeIteratorTraits<T, Traits>::next(this->nodes_, this->indices_);
      return *this;
   }

   // operator++ overload
   virtual CompositeIterator operator++(int) {
      CompositeIterator<T, Traits> tmp(*this);

      ++(*this);
      return *this;
   }

   template <class N>
   bool operator!=(const CompositeIterator<T, N>& itr) {
      bool this_empty = this->nodes_.empty();
      bool other_empty = itr.nodes_.empty();

      return (this_empty ^ other_empty) || (!this_empty && !other_empty && (this->nodes_.back() != itr.nodes_.back()));
   }

   template <class N>
   bool operator==(const CompositeIterator<T, N>& itr) {
      bool this_empty = this->nodes_.empty();
      bool other_empty = itr.nodes_.empty();

      return (this_empty && other_empty) || (!this_empty && !other_empty && (this->nodes_.back() == itr.nodes_.back()));
   }

   virtual T* operator*() {
      return (this->nodes_.empty() ? nullptr : CompositeIteratorTraits<T, Traits>::get(this->nodes_, this->indices_));
   }

   virtual T* operator->() {
      return (this->nodes_.empty() ? nullptr : CompositeIteratorTraits<T, Traits>::get(this->nodes_, this->indices_));
   }

protected:
   IndexList indices_;
   NodeList<T> nodes_;
};

// --------------------------------------------------------------------------
// Composite constructor implementations
// --------------------------------------------------------------------------
template <class T, typename Traits>
Composite<T, Traits>::Composite() {
}

template <class T, typename Traits>
Composite<T, Traits>::~Composite() {
   this->children_.clear();
}

// --------------------------------------------------------------------------
// Composite method implementations
// --------------------------------------------------------------------------
template <class T, typename Traits>
void Composite<T, Traits>::add(T* child) {
   if (!child) {
      return;
   }

   this->add_pre(child);
   this->children_.push_back(child);
   this->add_post(child);
}

template <class T, typename Traits>
T* Composite<T, Traits>::get(int idx) {
   if (idx < 0 || idx >= this->get_num_children()) {
      return nullptr;
   }

   this->get_pre(idx);
   return this->children_[idx];
   this->get_post(idx);
}

template <class T, typename Traits>
void Composite<T, Traits>::remove(T* child) {
   this->remove_pre(child);

   typename std::vector<T*>::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if (*it == child) {
         this->children_.erase(it);
         return;
      }
   }

   this->remove_post(child);
}

template <class T, typename Traits>
void Composite<T, Traits>::remove(int idx) {
   if (idx < 0 || idx >= this->get_num_children()) {
      return;
   }

   this->remove_pre(idx);
   this->children_.erase(this->children_.begin() + idx);
   this->remove_post(idx);
}

template <class T, typename Traits>
int Composite<T, Traits>::get_num_children() {
   return this->children_.size();
}

template <class T, typename Traits>
CompositeIterator<T, Traits> Composite<T, Traits>::begin() {
   CompositeIterator<T, Traits> tmp(static_cast<T*>(this));
   return tmp;
}

template <class T, typename Traits>
CompositeIterator<T, Traits> Composite<T, Traits>::end() {
   CompositeIterator<T, Traits> tmp;
   return tmp;
}

#endif
