#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>

template <class T>
class IteratorStrategy {
public:
   virtual T* increment(T* node) = 0;
   virtual T* decrement(T* node) = 0;
};

// abstract iterator class for customized iterators
template <class T>
class Iterator
: public std::iterator<std::random_access_iterator_tag, T>
{
public:
   Iterator();
   Iterator(T* itr);

   virtual Iterator& operator++() = 0;    // prefix
   virtual Iterator& operator++(int) = 0; // postfix

   virtual bool operator!=(const Iterator& itr) = 0;
   virtual bool operator==(const Iterator& itr) = 0;

   virtual T operator*() = 0;
   virtual T operator->() = 0;

protected:
   std::unique_ptr<IteratorStrategy> strategy_;
};

// abstract interface for containers that can be iterated on
class Iterable {
public:
   virtual Iterator& begin() = 0;
   virtual Iterator& end() = 0;
};

#endif
