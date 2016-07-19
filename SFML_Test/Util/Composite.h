// -----------------------------------------------------------------------------
// Composite.h
//
// This is a generic implementation of the composite OOP design pattern in C++.
// Also in this file are a few basic iterators (prefix, postfix, breadth)
// for traversing a composite structure in a tree-like manner.
//
// Cory Parsnipson
// -----------------------------------------------------------------------------

#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <iostream>
#include <vector>
#include <iterator>

// -----------------------------------------------------------------------------
// forward declarations
// -----------------------------------------------------------------------------
class TraitsShallow;
class TraitsPrefix;
class TraitsPostfix;
class TraitsBreadth;

template <class T, bool iterateInReverse, class Traits> class CompositeIterator;

// -----------------------------------------------------------------------------
// aliases
// -----------------------------------------------------------------------------
using IndexList = std::vector<int>;

template <class T>
using NodeList = std::vector<T*>;

// -----------------------------------------------------------------------------
// Composite abstract base
//
// This is an archetype specification for classes where instances may have
// any number of children of the same inheritance family.
//
// Concrete implementations should only override methods under the "hooks"
// section of the class definition.
// -----------------------------------------------------------------------------
template <class T, bool hasParentPointer = false>
class Composite
{
public:
   using iterator                       = CompositeIterator<T, false, TraitsShallow>;
   using const_iterator                 = CompositeIterator<const T, false, TraitsShallow>;
   using reverse_iterator               = CompositeIterator<T, true, TraitsShallow>;
   using const_reverse_iterator         = CompositeIterator<const T, true, TraitsShallow>;

   using prefix_iterator                = CompositeIterator<T, false, TraitsPrefix>;
   using const_prefix_iterator          = CompositeIterator<const T, false, TraitsPrefix>;
   using reverse_prefix_iterator        = CompositeIterator<T, true, TraitsPrefix>;
   using const_reverse_prefix_iterator  = CompositeIterator<const T, true, TraitsPrefix>;

   using postfix_iterator               = CompositeIterator<T, false, TraitsPostfix>;
   using const_postfix_iterator         = CompositeIterator<const T, false, TraitsPostfix>;
   using reverse_postfix_iterator       = CompositeIterator<T, true, TraitsPostfix>;
   using const_reverse_postfix_iterator = CompositeIterator<const T, true, TraitsPostfix>;

   using breadth_iterator               = CompositeIterator<T, false, TraitsBreadth>;
   using const_breadth_iterator         = CompositeIterator<const T, false, TraitsBreadth>;
   using reverse_breadth_iterator       = CompositeIterator<T, true, TraitsBreadth>;
   using const_reverse_breadth_iterator = CompositeIterator<const T, true, TraitsBreadth>;

   Composite();
   virtual ~Composite() = 0;

   // client interface
   void add(T* child);
   void insert(int idx, T* child);

   T* child(int idx) const;
   int num_children() const;

   void remove(T* child);
   void remove(int idx);

   T* parent() const;

   // hooks for child interface
   virtual void add_pre(T* child) {}
   virtual void add_post(T* child) {}

   virtual void insert_pre(int idx, T* child) {}
   virtual void insert_post(int idx, T* child) {}

   virtual void child_pre(int idx) const {}
   virtual void child_post(int idx) const {}

   virtual void remove_pre(T* child) {}
   virtual void remove_post(T* child) {}

   virtual void remove_pre(int idx) {}
   virtual void remove_post(int idx) {}

   virtual void parent_pre(T* parent) {}
   virtual void parent_post(T* parent) {}

   // iterator interface
   virtual CompositeIterator<T, false, TraitsPrefix> begin();
   virtual CompositeIterator<T, false, TraitsPrefix> end();

   virtual CompositeIterator<const T, false, TraitsPrefix> begin() const;
   virtual CompositeIterator<const T, false, TraitsPrefix> end() const;

   virtual CompositeIterator<T, true, TraitsPrefix> rbegin();
   virtual CompositeIterator<T, true, TraitsPrefix> rend();

   virtual CompositeIterator<const T, true, TraitsPrefix> rbegin() const;
   virtual CompositeIterator<const T, true, TraitsPrefix> rend() const;

protected:
   void parent(T* parent);

private:
   T* parent_;
   std::vector<T*> children_;

};

// -----------------------------------------------------------------------------
// Composite iterator trait definitions
// -----------------------------------------------------------------------------
template <class T, bool iterateInReverse, class Traits>
class CompositeIteratorTraits;

template <class T, bool iterateInReverse>
class CompositeIteratorTraits<T, iterateInReverse, TraitsShallow> {
public:
   // any initialization steps necessary
   static void init(NodeList<T>& nodes, IndexList& indices) {
      if (nodes.empty()) {
         return;
      }

      T* node = nodes[0];
      nodes.erase(nodes.begin());
      
      int num_nodes = node->num_children();
      int i = 0;

      while (i < num_nodes) {
         nodes.push_back(node->child(iterateInReverse ? num_nodes - 1 - i : i));
         ++i;
      }
   }

   // retrive current node in iteration
   static T* get(NodeList<T>& nodes, IndexList& indices) {
      return (nodes.empty() ? nullptr : nodes.front());
   }

   // increment the current node in the iteration
   static void next(NodeList<T>& nodes, IndexList& indices) {
      if (!nodes.empty()) {
         nodes.erase(nodes.begin());
      }
   }
};

template <class T, bool iterateInReverse>
class CompositeIteratorTraits<T, iterateInReverse, TraitsPrefix> {
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
      if (!nodes.empty() && indices.back() < nodes.back()->num_children()) {
         int next_idx = iterateInReverse ? nodes.back()->num_children() - 1 - indices.back() : indices.back();

         nodes.push_back(nodes.back()->child(next_idx));
         indices.push_back(0);
         return;
      }

      // we have reached a leaf node or finished visiting all children
      while (!nodes.empty() && indices.back() == nodes.back()->num_children()) {
         // pop current node because we have already visited this node
         // (because we are doing prefix iteration)
         indices.pop_back();
         nodes.pop_back();

         // do the if statement from above here
         // NOTE: indices should be the same length as nodes
         if (!nodes.empty() && ++indices.back() < nodes.back()->num_children()) {
            int next_idx = iterateInReverse ? nodes.back()->num_children() - 1 - indices.back() : indices.back();

            nodes.push_back(nodes.back()->child(next_idx));
            indices.push_back(0);
            break;
         }
      }
   }
};

template <class T, bool iterateInReverse>
class CompositeIteratorTraits<T, iterateInReverse, TraitsPostfix> {
public:
   // any initialization steps necessary
   static void init(NodeList<T>& nodes, IndexList& indices) {
      if (nodes.empty()) {
         return;
      }
   
      // move to the first leaf node
      while (indices.back() < nodes.back()->num_children()) {
         int next_idx = iterateInReverse ? nodes.back()->num_children() - 1 - indices.back() : indices.back();

         nodes.push_back(nodes.back()->child(next_idx));
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

      if (indices.back() < nodes.back()->num_children()) {
         // we still have children to traverse
         int next_idx = iterateInReverse ? nodes.back()->num_children() - 1 - indices.back() : indices.back();

         nodes.push_back(nodes.back()->child(next_idx));
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
         while (indices.back() < nodes.back()->num_children()) {
            int next_idx = iterateInReverse ? nodes.back()->num_children() - 1 - indices.back() : indices.back();

            nodes.push_back(nodes.back()->child(next_idx));
            indices.push_back(0);
         }
      }
   }
};

template <class T, bool iterateInReverse>
class CompositeIteratorTraits<T, iterateInReverse, TraitsBreadth> {
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
      for (int i = 0; i < nodes.front()->num_children(); ++i) {
         nodes.push_back(nodes.front()->child(iterateInReverse ? nodes.front()->num_children() - 1 - i : i));
      }

      // pop the front of the nodelist (it was the old current node)
      nodes.erase(nodes.begin());
   }
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
template <class T, bool iterateInReverse = false, typename Traits = TraitsPrefix>
class CompositeIterator
: public std::iterator<std::bidirectional_iterator_tag, T>
{
public:
   // NOTE: need to add a line for each type of trait
   friend class CompositeIterator<T, false, TraitsShallow>;
   friend class CompositeIterator<const T, false, TraitsShallow>;
   friend class CompositeIterator<T, true, TraitsShallow>;
   friend class CompositeIterator<const T, true, TraitsShallow>;

   friend class CompositeIterator<T, false, TraitsPrefix>;
   friend class CompositeIterator<const T, false, TraitsPrefix>;
   friend class CompositeIterator<T, true, TraitsPrefix>;
   friend class CompositeIterator<const T, true, TraitsPrefix>;

   friend class CompositeIterator<T, false, TraitsPostfix>;
   friend class CompositeIterator<const T, false, TraitsPostfix>;
   friend class CompositeIterator<T, true, TraitsPostfix>;
   friend class CompositeIterator<const T, true, TraitsPostfix>;

   friend class CompositeIterator<T, false, TraitsBreadth>;
   friend class CompositeIterator<const T, false, TraitsBreadth>;
   friend class CompositeIterator<T, true, TraitsBreadth>;
   friend class CompositeIterator<const T, true, TraitsBreadth>;

   CompositeIterator() {
      CompositeIteratorTraits<T, iterateInReverse, Traits>::init(this->nodes_, this->indices_);
   }

   CompositeIterator(T* itr) {
      this->indices_.push_back(0);
      this->nodes_.push_back(itr);
      
      CompositeIteratorTraits<T, iterateInReverse, Traits>::init(this->nodes_, this->indices_);
   }

   template <typename constT>
   CompositeIterator(constT* itr) {
      this->indices_.push_back(0);
      this->nodes_.push_back(itr);
      
      CompositeIteratorTraits<constT, iterateInReverse, Traits>::init(this->nodes_, this->indices_);
   }

   CompositeIterator(const CompositeIterator<T, iterateInReverse, Traits>& other)
   : indices_(other.indices_)
   , nodes_(other.nodes_)
   {
      CompositeIteratorTraits<T, iterateInReverse, Traits>::init(this->nodes_, this->indices_);
   }

   // assignment operator overload
   // NOTE: needed to convert between CompositeIterators with different traits
   template <typename constT>
   CompositeIterator operator=(CompositeIterator<constT, iterateInReverse, TraitsPrefix> other) {
      // check for self assignment and skip if so
      if ((void*)this != (void*)&other) {
         typename NodeList<constT>::const_iterator it;
         for (it = other.nodes_.begin(); it != other.nodes_.end(); ++it) {
            this->nodes_.push_back(*it);
         }

         this->indices_ = other.indices_;
      }

      CompositeIteratorTraits<T, iterateInReverse, Traits>::init(this->nodes_, this->indices_);
      return *this;
   }

   // ++operator overload
   virtual CompositeIterator& operator++() {
      CompositeIteratorTraits<T, iterateInReverse, Traits>::next(this->nodes_, this->indices_);
      return *this;
   }

   // operator++ overload
   virtual CompositeIterator operator++(int) {
      CompositeIterator<T, iterateInReverse, Traits> tmp(*this);

      ++(*this);
      return *this;
   }

   template <typename constT, typename OtherTraits>
   bool operator!=(const CompositeIterator<constT, iterateInReverse, OtherTraits>& itr) {
      bool this_empty = this->nodes_.empty();
      bool other_empty = itr.nodes_.empty();

      return (this_empty ^ other_empty) || (!this_empty && !other_empty && (this->nodes_.back() != itr.nodes_.back()));
   }

   template <typename constT, typename OtherTraits>
   bool operator==(const CompositeIterator<constT, iterateInReverse, OtherTraits>& itr) {
      bool this_empty = this->nodes_.empty();
      bool other_empty = itr.nodes_.empty();

      return (this_empty && other_empty) || (!this_empty && !other_empty && (this->nodes_.back() == itr.nodes_.back()));
   }

   virtual T* operator*() {
      return (this->nodes_.empty() ? nullptr : CompositeIteratorTraits<T, iterateInReverse, Traits>::get(this->nodes_, this->indices_));
   }

   virtual T* operator->() {
      return (this->nodes_.empty() ? nullptr : CompositeIteratorTraits<T, iterateInReverse, Traits>::get(this->nodes_, this->indices_));
   }

protected:
   IndexList indices_;
   NodeList<T> nodes_;
};

// --------------------------------------------------------------------------
// Composite constructor implementations
// --------------------------------------------------------------------------
template <class T, bool hasParentPointer>
Composite<T, hasParentPointer>::Composite()
: parent_(nullptr)
{
}

template <class T, bool hasParentPointer>
Composite<T, hasParentPointer>::~Composite() {
   this->children_.clear();

   // remove this from parent
   this->parent(nullptr);
}

// --------------------------------------------------------------------------
// Composite method implementations
// --------------------------------------------------------------------------
template <class T, bool hasParentPointer>
void Composite<T, hasParentPointer>::add(T* child) {
   if (!child) {
      return;
   }

   this->add_pre(child);
   this->children_.push_back(child);

   // update parent pointer if necessary
   if (hasParentPointer) {
      child->parent(static_cast<T*>(this));
   }

   this->add_post(child);
}

template <class T, bool hasParentPointer>
void Composite<T, hasParentPointer>::insert(int idx, T* child) {
   typename std::vector<T*>::const_iterator it;

   if (idx <= 0) {
      it = this->children_.begin();
   } else if (idx >= (signed int)this->children_.size()) {
      it = this->children_.end();
   } else {
      it = this->children_.begin() + idx;
   }

   this->insert_pre(idx, child);
   this->children_.insert(it, child);

   // update parent pointer if necessary
   if (hasParentPointer) {
      child->parent(static_cast<T*>(this));
   }

   this->insert_post(idx, child);
}

template <class T, bool hasParentPointer>
T* Composite<T, hasParentPointer>::child(int idx) const {
   if (idx < 0 || idx >= this->num_children()) {
      return nullptr;
   }

   this->child_pre(idx);
   return this->children_[idx];
   this->child_post(idx);
}

template <class T, bool hasParentPointer>
int Composite<T, hasParentPointer>::num_children() const {
   return this->children_.size();
}

template <class T, bool hasParentPointer>
void Composite<T, hasParentPointer>::remove(T* child) {
   if (!child) {
      return;
   }

   this->remove_pre(child);

   typename std::vector<T*>::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if (*it == child) {
         this->children_.erase(it);
         break;
      }
   }

   // remove child's parent pointer
   child->parent(nullptr);

   this->remove_post(child);
}

template <class T, bool hasParentPointer>
void Composite<T, hasParentPointer>::remove(int idx) {
   if (idx < 0 || idx >= this->num_children()) {
      return;
   }

   this->remove_pre(idx);
   this->children_.erase(this->children_.begin() + idx);

   // remove child's parent pointer
   *(this->children_.begin() + idx)->parent(nullptr);

   this->remove_post(idx);
}

template <class T, bool hasParentPointer>
void Composite<T, hasParentPointer>::parent(T* parent) {
   if (!hasParentPointer) {
      return;
   }

   // WARNING: this does not remove child from parent
   this->parent_pre(parent);
   this->parent_ = parent;
   this->parent_post(parent);
}

template <class T, bool hasParentPointer>
T* Composite<T, hasParentPointer>::parent() const {
   return this->parent_;
}

template <class T, bool hasParentPointer>
CompositeIterator<T, false, TraitsPrefix> Composite<T, hasParentPointer>::begin() {
   CompositeIterator<T, false, TraitsPrefix> tmp(static_cast<T*>(this));
   return tmp;
}

template <class T, bool hasParentPointer>
CompositeIterator<T, false, TraitsPrefix> Composite<T, hasParentPointer>::end() {
   CompositeIterator<T, false, TraitsPrefix> tmp;
   return tmp;
}

template <class T, bool hasParentPointer>
CompositeIterator<const T, false, TraitsPrefix> Composite<T, hasParentPointer>::begin() const {
   CompositeIterator<const T, false, TraitsPrefix> tmp(static_cast<const T*>(this));
   return tmp;
}

template <class T, bool hasParentPointer>
CompositeIterator<const T, false, TraitsPrefix> Composite<T, hasParentPointer>::end() const {
   CompositeIterator<const T, false, TraitsPrefix> tmp;
   return tmp;
}

template <class T, bool hasParentPointer>
CompositeIterator<T, true, TraitsPrefix> Composite<T, hasParentPointer>::rbegin() {
   CompositeIterator<T, true, TraitsPrefix> tmp(static_cast<T*>(this));
   return tmp;
}

template <class T, bool hasParentPointer>
CompositeIterator<T, true, TraitsPrefix> Composite<T, hasParentPointer>::rend() {
   CompositeIterator<T, true, TraitsPrefix> tmp;
   return tmp;
}

template <class T, bool hasParentPointer>
CompositeIterator<const T, true, TraitsPrefix> Composite<T, hasParentPointer>::rbegin() const {
   CompositeIterator<const T, true, TraitsPrefix> tmp(static_cast<const T*>(this));
   return tmp;
}

template <class T, bool hasParentPointer>
CompositeIterator<const T, true, TraitsPrefix> Composite<T, hasParentPointer>::rend() const {
   CompositeIterator<const T, true, TraitsPrefix> tmp;
   return tmp;
}

#endif
